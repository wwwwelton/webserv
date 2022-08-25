#pragma once
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "Request.hpp"
#include "Logger.hpp"
#include "defines.hpp"

#include <exception>
#include <string>
#include <vector>

enum RequestHeaderStates {
  S_INIT = 1,

  S_METHOD_START,
  S_METHOD,
  S_URI_START,
  S_URI,
  S_HTTP_VERSION,

  S_REQUEST_LINE_CRLF,
  S_REQUEST_LINE_CR,
  S_REQUEST_LINE_LF,

  S_HEADER_LINE_START,
  S_HEADERS_END_CRLF,
  S_HEADERS_END_CR,
  S_HEADERS_END_LF,

  S_HEADER_LINE_KEY,
  S_HEADER_LINE_SPACE,
  S_HEADER_LINE_VALUE,
  S_HEADER_LINE_CRLF,
  S_HEADER_LINE_CR,
  S_HEADER_LINE_LF,
  S_HEADER_FINISHED
};

enum RequestChunkStates {
  S_BODY_INIT = 200,
  // S_BODY_START,
  // S_BODY,
  // S_BODY_LF,

  S_CHUNK_INIT,
  S_CHUNK_START,
  S_CHUNK_SIZE,
  S_CHUNK_EXTENSIONS,
  S_CHUNK_SIZE_LF,
  S_CHUNK_DATA_START,
  S_CHUNK_DATA,
  S_CHUNK_DATA_LF,
  S_LAST_CHUNK,
  S_LAST_CHUNK_LF,
  S_CHUNK_END,
  S_CHUNK_END_LF
};

enum ParsingResult {
  P_PARSING_COMPLETE,
  P_PARSING_INCOMPLETE,
  P_PARSING_INVALID,
  P_HEADER_COMPLETE,
  P_REQUEST_COMPLETE
};

enum RequestErrors {
  BadRequest = BAD_REQUEST,
  MethodNotAllowed = METHOD_NOT_ALLOWED,
  RequestTimeout = REQUEST_TIMEOUT,
  LengthRequired = LENGTH_REQUIRED,
  RequestEntityTooLarge = REQUEST_ENTITY_TOO_LARGE,
  RequestUriTooLong = REQUEST_URI_TOO_LONG,
  HttpVersionUnsupported = HTTP_VERSION_UNSUPPORTED
};

class RequestParser
{
  RequestParser(const RequestParser &);
  RequestParser &operator=(const RequestParser &);
public:
  int fd;
  bool finished;

  RequestParser(int fd = -1, size_t max_body_size = 0, size_t buffer_max = 65536);
  ~RequestParser();

  bool is_connected() const;
  bool is_header_finished() const;

  void parse_header();

  void prepare_chunk();
  bool is_chunk_ready() const;
  const std::vector<char>& get_chunk();

  Request &get_request();
  void reset();

  class InvalidRequestException: public std::exception {
    RequestErrors _error;
  public:
    InvalidRequestException(RequestErrors error);
    const char* what() const throw();
    RequestErrors get_error() const;
  };

  class HeaderFinishedException: public std::exception {
  public:
    const char* what() const throw();
  };

  class RequestFinishedException: public std::exception {
  public:
    const char* what() const throw();
  };

  class ConnectionClosedException: public std::exception {
  public:
    const char* what() const throw();
  };

  class ReadException: public std::exception {
    std::string _message;
  public:
    ReadException(const std::string& message);
    const char* what() const throw();
    virtual ~ReadException() throw();
  };

private:
  bool valid;
  bool connected;

  bool header_finished;
  bool just_finished_header;

  size_t content_length;
  size_t max_content_length;
  size_t body_bytes_so_far;
  bool parsing_body;

  bool chunked;
  size_t chunk_size;
  bool chunk_ready;
  std::vector<char> chunk_data;

  Request *_request;

  Logger& log;

  // buffer iterator;
  size_t i;
  char *buffer;
  size_t bytes_read;
  size_t buffer_size;

  std::string _header_key;
  std::string _header_value;

  RequestHeaderStates header_state;
  RequestChunkStates chunk_state;

  static std::string supported_version;
  size_t supported_version_index;

  ParsingResult tokenize_partial_request(char *buff);
  ParsingResult tokenize_header(char *buff);
  ParsingResult tokenize_chunk_size(char *buff);

  bool prepare_chunked_body();
  bool prepare_regular_body();

  void check_read_value(size_t bytes_read);

  // utils
  std::ostream& debug();
  std::ostream& info();
  std::ostream& warning();
  std::ostream& error();

};

#endif // !HTTP_REQUEST_PARSER_HPP
