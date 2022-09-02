#include "RequestParser.hpp"
#include "Request.hpp"
#include "WebServ.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <exception>

static inline bool is_space(char c) {
  return (c == ' ');
}

static inline bool is_lalpha(char c) {
  return (c >= 'a' && c <= 'z');
}

static inline bool is_ualpha(char c) {
  return (c >= 'A' && c <= 'Z');
}

static inline bool is_alpha(char c) {
  return (is_ualpha(c) || is_lalpha(c));
}

static inline bool is_ctl(char c) {
  return ((c >= 0 && c < 32) || c == 127);
}

static inline bool is_character(char c) {
  return (std::isdigit(c) || is_alpha(c));
}

static inline int get_hex(char c) {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return (c - '0');
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      return (c - 'A' + 10);
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      return (c - 'a' + 10);
    default:
      return -1;
  }
}

static inline bool is_separator(char c) {
  switch (c) {
    case '(':
    case ')':
    case '<':
    case '>':
    case '@':
    case ',':
    case ';':
    case ':':
    case '"':
    case '/':
    case '[':
    case ']':
    case '?':
    case '=':
    case '{':
    case '}':
    case ' ':
    case '\\':
    case '\t':
      return true;
    default:
      return false;
  }
}

static inline bool is_ascii(char c) {
  return (c >= 0);
}

static inline bool is_token(char c) {
  return (is_ascii(c) && !is_ctl(c) && !is_separator(c));
}

static inline bool str_iequals(const std::string& str1, const std::string& str2) {
  size_t size1 = str1.size();

  if (size1 != str2.size())
    return false;

  const char *cstr1 = str1.c_str();
  const char *cstr2 = str2.c_str();

  for (size_t i = 0; i < size1; i++) {
    if (std::tolower(cstr1[i]) != std::tolower(cstr2[i]))
      return false;
  }
  return true;
}

std::string RequestParser::supported_version = "HTTP/1.1";

RequestParser::RequestParser(int fd, size_t max_body_size, size_t buffer_size):
  fd(fd),
  finished(false),
  valid(false),
  connected(true),
  header_finished(false),
  content_length(),
  max_content_length(max_body_size),
  body_bytes_so_far(),
  parsing_body(false),
  chunked(false),
  chunk_size(),
  chunk_ready(false),
  log(WebServ::log),
  buffer(new char[buffer_size]),
  bytes_read(),
  buffer_size(buffer_size),
  header_state(S_INIT),
  chunk_state(S_CHUNK_INIT),
  supported_version_index(0) {
    _request = new Request();
    just_finished_header = false;
  }

RequestParser::RequestParser(const RequestParser &): log(WebServ::log) { }

RequestParser& RequestParser::operator=(const RequestParser &) { return *this; }

RequestParser::~RequestParser() {
  delete[] buffer;
  delete _request;
}

void print_chunk(std::ostream& out, const char *str, size_t start, size_t size) {
  for (size_t i = start; size > 0; size--) {
    char c = str[i++];
    if (std::isprint(c)) {
      out << c;
    } else {
      if (c == '\r')
        out << "\\r";
      else if (c == '\n')
        out << "\\n";
      else
        out << '.';
    }
  }
  out << std::endl;
}

/*
 * the request header starts with
 * METHOD /uri.php HTTP/1.1\r\n
 * */

ParsingResult RequestParser::tokenize_header(char *buff) {
  i = 0;

  while (i < bytes_read) {
    char c = buff[i++];

    switch (header_state) {
      case S_INIT:
      case S_METHOD_START:
        if (!is_ualpha(c)) {
          throw InvalidRequestException(BadRequest);
        }
        _request->method.push_back(c);
        header_state = S_METHOD;
        break;

      case S_METHOD:
        if (is_space(c)) {
          header_state = S_URI_START;
        } else if (!is_ualpha(c)) {
          throw InvalidRequestException(BadRequest);
        } else {
          _request->method.push_back(c);
        }
        break;

      case S_URI_START:
        if (c != '/') {
          throw InvalidRequestException(BadRequest);
        }
        _request->path.push_back(c);
        header_state = S_URI;
        break;

      case S_URI:
        if(is_space(c)) {
          header_state = S_HTTP_VERSION;
        } else if (is_ctl(c)) {
          throw InvalidRequestException(BadRequest);
        } else {
          _request->path.push_back(c);
        }
        break;

      case S_HTTP_VERSION:
        {
          size_t idx = supported_version_index;
          if (idx >= supported_version.size()) {
            _request->http_version = supported_version;
            header_state = S_REQUEST_LINE_CRLF;
          } else if (c != supported_version[idx]) {
            RequestErrors error = idx > 4 ? HttpVersionUnsupported : BadRequest;
            throw InvalidRequestException(error);
          } else {
            supported_version_index++;
          }
        }
        break;

      case S_REQUEST_LINE_CRLF:
        if (c == '\r') {
          header_state = S_REQUEST_LINE_LF;
        } else if (c == '\n') {
          header_state = S_HEADER_LINE_START;
        } else
          throw InvalidRequestException(BadRequest);
        break;

      case S_REQUEST_LINE_LF:
        if (c == '\n') {
          header_state = S_HEADER_LINE_START;
        } else
            throw InvalidRequestException(BadRequest);
        break;

      case S_HEADER_LINE_START:
        if (c == '\r') {
          header_state = S_HEADERS_END_LF;
        } else if (c == '\n') {
          header_finished = true;
          header_state = S_HEADER_FINISHED;
          if (_request->method == "GET")
            return P_PARSING_COMPLETE;
        } else if (!is_character(c)) {
            throw InvalidRequestException(BadRequest);
        } else {
          _header_key.clear();
          _header_key.push_back(c);
          header_state = S_HEADER_LINE_KEY;
        }
        break;

      case S_HEADER_LINE_KEY:
        if (c == ':') {
          _request->headers[_header_key];
          header_state = S_HEADER_LINE_SPACE;
        } else if (is_ctl(c) || is_separator(c)) {
          throw InvalidRequestException(BadRequest);
        } else {
          _header_key.push_back(c);
        }
        break;

      case S_HEADER_LINE_SPACE:
        if (c != ' ') {
          throw InvalidRequestException(BadRequest);
        } else {
          _header_value.clear();
          header_state = S_HEADER_LINE_VALUE;
        }
        break;

      case S_HEADER_LINE_VALUE:
        if (c == '\r' || c == '\n') {
          if (str_iequals(_header_key, "content-length")) {
            _header_key = "Content-Length";
            std::stringstream ss(_header_value);
            ss >> content_length;
            if (content_length > max_content_length) {
              warning() << "request content-length is "
                << content_length
                << " but the serve max content-length acceptable is "
                << max_content_length
                << std::endl;
              throw InvalidRequestException(RequestEntityTooLarge);
            }
          } else if (str_iequals(_header_key, "transfer-encoding")) {
            _header_key = "Transfer-Encoding";
            if (_header_value != "identity")
              chunked = true;
          }
          _request->headers[_header_key] = _header_value;
        }
        if (c == '\r') { // header value finished
          header_state = S_HEADER_LINE_LF;
        } else if (c == '\n') {
          header_state = S_HEADER_LINE_START; // check for a new header
        } else if (is_ctl(c)) {
          throw InvalidRequestException(BadRequest);
        } else {
          _header_value.push_back(c);
          header_state = S_HEADER_LINE_VALUE;
        }
        break;

      case S_HEADER_LINE_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);

        _request->headers[_header_key] = _header_value;
        header_state = S_HEADER_LINE_START; // check for a new header
        break;

      case S_HEADERS_END_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);
        header_finished = true;
        header_state = S_HEADER_FINISHED;
        if (_request->method == "GET") {
          finished = true;
          return P_PARSING_COMPLETE;
        } else if (chunked) {
          chunk_state = S_CHUNK_START;
          parsing_body = true;
        } else if (content_length == 0) {
          return P_PARSING_COMPLETE;
        } else {
          chunk_state = S_BODY_INIT;
          parsing_body = true;
        }
        return P_PARSING_COMPLETE;
        break;
      default:
        throw std::exception();
        break;
    }
  }
  return P_PARSING_INCOMPLETE;
}

ParsingResult RequestParser::tokenize_chunk_size(char *buff) {

  while (i < bytes_read) {
    char c = buff[i++];

    switch (chunk_state) {

      case S_CHUNK_START:
        chunk_size = get_hex(c);
        if (chunk_size == (size_t)-1) {
          throw InvalidRequestException(BadRequest);
        }
        if (chunk_size == 0)
          chunk_state = S_LAST_CHUNK;
        else
          chunk_state = S_CHUNK_SIZE;
        break;

      case S_CHUNK_SIZE:
        {
          if (c == ';') {
            chunk_state = S_CHUNK_EXTENSIONS;
          } else  if (c == '\r') {
            if (chunk_size == 0) {
              chunk_state = S_LAST_CHUNK_LF;
            } else {
              chunk_state = S_CHUNK_SIZE_LF;
            }

          } else {
            int _hex = get_hex(c);

            if (_hex == -1) {
              throw InvalidRequestException(BadRequest);

            } else {
              chunk_size = chunk_size * 16 + _hex;
            }
          }
          break ;
        }

      case S_CHUNK_EXTENSIONS:
        if (c == '\r') {
          chunk_state = S_CHUNK_SIZE_LF;
        } else if (c == '\n') {
          chunk_state = S_CHUNK_DATA;
        } else if (!is_token(c))
          throw InvalidRequestException(BadRequest);
        break;

      case S_CHUNK_SIZE_LF:
        if (c != '\n') {
          throw InvalidRequestException(BadRequest);
        }
        chunk_state = S_CHUNK_DATA;
        info() << "finished reading chunk size: " << chunk_size << '\n';
        body_bytes_so_far += chunk_size;
        if (body_bytes_so_far > max_content_length) {
          error() << "client is sending a chunked request, but the request"
            << " reached the server max tolerance\n"
            << "current request size is " << body_bytes_so_far
            << " but the configured client_max_body_size is "
            << max_content_length << "\n";
          throw InvalidRequestException(RequestEntityTooLarge);
        }
        break;

      case S_CHUNK_DATA:
        if (chunk_size == 0) {
          if (c != '\r') {
            throw InvalidRequestException(BadRequest);
          }
          chunk_state = S_CHUNK_DATA_LF;
        } else {
          // there would be a push_back(c) here;
          int read_size = std::min(chunk_size, bytes_read - i + 1);
          // chunk_size--;
          int start = i - 1;
          chunk_data.insert(
              chunk_data.end(),
              buffer + start,
              buffer + start + read_size);
          chunk_size -= read_size;
          i += read_size - 1;
          // chunk_data.push_back(c);
        }
        break;

      case S_CHUNK_DATA_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);
        chunk_state = S_CHUNK_START;
        break;

      case S_LAST_CHUNK:
        if (c != '\r')
          throw InvalidRequestException(BadRequest);
        chunk_state = S_LAST_CHUNK_LF;
        break;

      case S_LAST_CHUNK_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);
        chunk_state = S_CHUNK_END;
        break;

      case S_CHUNK_END:
        if (c != '\r')
          throw InvalidRequestException(BadRequest);
        chunk_state = S_CHUNK_END_LF;
        break;

      case S_CHUNK_END_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);
        finished = true;
        return P_PARSING_COMPLETE;
        break;

      default:
        throw std::exception();
    }
  }
  return P_PARSING_INCOMPLETE;
}

void RequestParser::parse_header() {
  if (header_finished)
    throw HeaderFinishedException();
  if (!connected)
    throw ConnectionClosedException();

  bytes_read = recv(fd, buffer, buffer_size, 0);

  check_read_value(bytes_read);
  info() << "bytes read: " << bytes_read << std::endl;

  try {
    ParsingResult result = tokenize_header(buffer);
    if (result == P_PARSING_COMPLETE) {
      if (!chunked && content_length == 0)
        finished = true;
      _request->error = 0;
      debug() << "finished request header: " << *_request << std::endl;
      just_finished_header = true;
    }
  } catch(InvalidRequestException& ex) {
    warning() << "invalid http request: " << ex.what() << std::endl;
    _request->error = ex.get_error();
    finished = true;
  } catch(std::exception& e) {
    error()
      << "unexpected exception on RequestParser: "
      << e.what() << std::endl;
    _request->error = 500;
    finished = true;
  }
}

void RequestParser::check_read_value(size_t bytes_read) {
  if (bytes_read == (size_t)-1) {
    throw ReadException(strerror(errno));
  } else if (bytes_read == 0) {
    this->header_finished = true;
    this->finished = true;
    this->connected = false;
    warning()
      << "read 0 bytes, setting connection as closed"
      << std::endl;
    throw ConnectionClosedException();
  }
}

bool RequestParser::prepare_chunked_body() {
  info() << "preparing a chunked body\n";

  if (i == bytes_read) {
    if (just_finished_header) {
      debug() << "must poll again\n";
      just_finished_header = false;
      return false;
    }
    info() << "reading a new chunk\n";
    bytes_read = recv(fd, buffer, buffer_size, 0);
    check_read_value(bytes_read);
    i = 0;
  } else
    info() << "using remaining chunk in the buffer\n";

  chunk_data.clear();

  ParsingResult result = tokenize_chunk_size(buffer);

  if (result == P_PARSING_COMPLETE) {
    finished = true;
    warning() << "finished the chunked request" << std::endl;
  }
  if (chunk_data.size() != 0) {
    return true;
  }

  return false;
}

bool RequestParser::prepare_regular_body() {
  info() << "preparing a regular body\n";

  if (i < bytes_read) {
    info() << "using remaining" << (bytes_read - i) << "bytes from buffer\n";
    chunk_data.assign(buffer + i, buffer + bytes_read);
    body_bytes_so_far = bytes_read - i;
  } else {
    if (just_finished_header) {
      debug() << "must poll again\n";
      just_finished_header = false;
      return false;
    }
    info() << "reading more bytes\n";
    bytes_read = recv(fd, buffer, buffer_size, 0);
    check_read_value(bytes_read);
    body_bytes_so_far += bytes_read;
    info() << bytes_read << " bytes where read" << std::endl;
    chunk_data.assign(buffer, buffer + bytes_read);
  }

  i = -1;

  if (content_length > 0) {
    if (body_bytes_so_far > content_length) {
      finished = true;
      throw InvalidRequestException(RequestEntityTooLarge);
    } else if (body_bytes_so_far == content_length) {
      info() << "all content-length was read" << std::endl;
      finished = true;
    }
  }
  return true;
}

void RequestParser::prepare_chunk() {
  try {
    if (finished)
      throw RequestFinishedException();
    if (!connected)
      throw ConnectionClosedException();

    debug() << "preparing chunk" << std::endl;
    bool ready;
    if (chunked)
      ready = prepare_chunked_body();
    else
      ready = prepare_regular_body();
    chunk_ready = ready;

  } catch(InvalidRequestException& ex) {
    info() << "invalid http request: " << ex.what() << std::endl;
    _request->error = ex.get_error();
    finished = true;

  } catch(std::exception& e) {
    error()
      << "unexpected exception on RequestParser: "
      << e.what() << std::endl;
    _request->error = 500;
    finished = true;
  }
}

bool RequestParser::is_chunk_ready() const {
  log.debug() << "chunk is ready? " << std::boolalpha << chunk_ready << std::endl;
  return chunk_ready;
}

const std::vector<char>& RequestParser::get_chunk() {
  info() << "returning chunk" << std::endl;
//   print_chunk(debug(), &*chunk_data.begin(), 0, chunk_data.size());
  info() << "current body size: " << body_bytes_so_far << std::endl;

  chunk_ready = false;
  return chunk_data;
}

bool RequestParser::is_connected() const {
  return this->connected;
}

Request &RequestParser::get_request() {
  _request->finished = this->finished;
  return *_request;
}

bool RequestParser::is_header_finished() const {
  return header_finished;
}

void RequestParser::reset() {
  debug() << "reseting..." << std::endl;
  delete this->_request;
  this->_request = new Request();

  valid = false;
  finished = false;

  header_finished = false;
  just_finished_header = false;

  content_length = 0;
  body_bytes_so_far = 0;
  parsing_body = 0;

  chunked = 0;
  chunk_size = 0;
  chunk_ready = 0;
  chunk_data.clear();

  // buffer iterator;
  i = 0;
  bytes_read = 0;

  _header_key = "";
  _header_value = "";

  header_state = S_INIT;
  chunk_state = S_CHUNK_INIT;

  supported_version_index = 0;
}

std::ostream& RequestParser::debug() {
  return log.debug() << "[Request parser]: ";
}

std::ostream& RequestParser::info() {
  return log.info() << "[Request parser]: ";
}

std::ostream& RequestParser::warning() {
  return log.warning() << "[Request parser]: ";
}

std::ostream& RequestParser::error() {
  return log.error() << "[Request parser]: ";
}

RequestParser::InvalidRequestException::InvalidRequestException(RequestErrors error) {
  _error = error;
}

const char *RequestParser::InvalidRequestException::what() const throw() {
  switch (_error) {
    case BadRequest:
      return "Bad request";
    case MethodNotAllowed:
      return "Method not allowed";
    case RequestTimeout:
      return "Request timeout";
    case LengthRequired:
      return "Length required";
    case RequestEntityTooLarge:
      return "Request entity too large";
    case RequestUriTooLong:
      return "Request URI too long";
    case HttpVersionUnsupported:
      return "HTTP version not supported";
    default:
      return "Unknown error";
  }
}

RequestErrors RequestParser::InvalidRequestException::get_error() const {
  return _error;
}

const char* RequestParser::RequestFinishedException::what() const throw()  {
  return "This request has already been finished";
}

RequestParser::ReadException::ReadException(const std::string& mes)
  : _message(std::string("Read returned an error: ") + mes) {

}

const char* RequestParser::ReadException::what() const throw()  {
  return _message.c_str();
}

const char* RequestParser::ConnectionClosedException::what() const throw()  {
  return "Connection closed";
}

RequestParser::ReadException::~ReadException() throw() { }

const char* RequestParser::HeaderFinishedException::what() const throw()  {
  return "The header of this request has already been parsed";
}
