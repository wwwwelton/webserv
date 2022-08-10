#include "RequestParser.hpp"
#include "Request.hpp"
#include "WebServ.hpp"
#include <cctype>
#include <cstddef>
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
  finished(false),
  valid(false),
  connected(true),
  header_finished(false),
  content_length(),
  max_content_length(max_body_size),
  bytes_consumed(),
  parsing_body(false),
  chunked(false),
  chunk_size(),
  log(WebServ::log),
  fd(fd),
  buffer(new char[buffer_size]),
  bytes_read(),
  buffer_size(buffer_size),
  header_state(S_INIT),
  chunk_state(S_CHUNK_INIT),
  supported_version_index(0) {
    _request = new Request();
  }

RequestParser::RequestParser(const RequestParser &): log(WebServ::log) { }

RequestParser& RequestParser::operator=(const RequestParser &) { return *this; }

RequestParser::~RequestParser() {
  delete[] buffer;
  delete _request;
}

void print_chunk(const char *str, size_t start, size_t size) {
  std::ostream& out = std::cout;
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
}

/*
 * the request header starts with
 * METHOD /uri.php HTTP/1.1\r\n
 * */

ParsingResult RequestParser::tokenize_header(char *buff) {
  size_t i = 0;

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
          _request->headers[_header_key] = _header_value;
          if (str_iequals(_header_key, "content-length")) {
            std::stringstream ss(_header_value);
            ss >> content_length;
            if (content_length > max_content_length) {
              log.warning() << "Request content-length is "
                << content_length
                << " but the serve max content-length acceptable is "
                << max_content_length
                << std::endl;
              throw InvalidRequestException(RequestEntityTooLarge);
            }
            _request->body.reserve(content_length);
          } else if (str_iequals(_header_key, "transfer-encoding")) {
            if (_header_value != "identity")
              chunked = true;
          }
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
        break;
      default:
        break;
    }
  }
  return P_PARSING_COMPLETE;
}

ParsingResult RequestParser::tokenize_partial_request(char *buff) {
  size_t i = 0;

  // buff[bytes_read] = '\0';
  // log.debug() << "Incoming request data: [";
  // print_chunk(buff, 0, bytes_read);
  // std::cout << "]" << std::endl;
  while (i < bytes_read) {
    char c = buff[i++];
    // log.info() << "current request: " << *_request << std::endl;

    switch (chunk_state) {
      // case S_BODY_START:
      //   log.debug() << "Request before body parsing: " << *_request << std::endl;
      //   content_length--;
      //   _request->body.push_back(c);
      //   chunk_state = S_BODY;
      //   break;

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
        break;

      case S_CHUNK_DATA:
        if (chunk_size == 0) {
          if (c != '\r') {
            throw InvalidRequestException(BadRequest);
          }
          chunk_state = S_CHUNK_DATA_LF;
        } else {
          chunk_size--;
          chunk_data.push_back(c);
        }
        break;

      case S_CHUNK_DATA_LF:
        if (c != '\n')
          throw InvalidRequestException(BadRequest);
        chunk_state = S_CHUNK_START;
        break;

      case S_LAST_CHUNK:
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
        return P_PARSING_COMPLETE;
        break;

      // body won't be a multipart payload
      // case S_BODY:
      //   content_length--;
      //   _request->body.push_back(c);
      //   if (content_length == 0)
      //     return P_PARSING_COMPLETE;
      //   break;

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

  if (bytes_read == (size_t)-1) {
    throw ReadException(strerror(errno));

  } else if (bytes_read == 0) {
    this->finished = true;
    this->connected = false;
    log.warning()
      << "RequestParser: read 0 bytes, setting connection as closed"
      << std::endl;
    return;
  }
  log.debug() << "Bytes read: " << bytes_read << std::endl;

  try {
    ParsingResult result = tokenize_header(buffer);
    if (result == P_PARSING_COMPLETE) {
      if (chunked) {
        _request->body.assign(chunk_data.begin(), chunk_data.end());
      }
      _request->error = 0;
      finished = true;
      log.debug() << "Finished request: " << *_request << std::endl;
    }
  } catch(InvalidRequestException& ex) {
    log.warning() << "Invalid http request: " << ex.what() << std::endl;
    _request->error = ex.get_error();
    finished = true;
  } catch(std::exception& e) {
    log.error()
      << "Unexpected exception on RequestParser: "
      << e.what() << std::endl;
    _request->error = 500;
    finished = true;
  }
}

void RequestParser::parse() {
  parse_header();
}

bool RequestParser::is_connected() const {
  return this->connected;
}

Request &RequestParser::get_request() {
  _request->finished = this->finished;
  return *_request;
}

void RequestParser::reset() {
  log.debug() << "RequestParser: reseting..." << std::endl;
  delete this->_request;
  this->_request = new Request();

  finished = false;
  header_finished = false;
  header_state = S_INIT;
  chunk_state = S_CHUNK_INIT;
  valid = false;
  supported_version_index = 0;
  content_length = 0;
  bytes_consumed = 0;
  parsing_body = false;
  chunked = false;
  chunk_size = 0;
  bytes_read = 0;
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
  return "Can't parse: connection closed";
}

RequestParser::ReadException::~ReadException() throw() { }

const char* RequestParser::HeaderFinishedException::what() const throw()  {
  return "The header of this request has already been parsed";
}
