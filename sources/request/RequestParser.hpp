#pragma once
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "Request.hpp"
#include "defines.hpp"

#include <exception>
#include <string>
#include <vector>

enum RequestStates {
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

  S_BODY_START,
  S_BODY,
  S_BODY_LF,

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
  S_CHUNK_END_LF,

  S_DEAD = 0
};

enum ParsingResult {
  P_PARSING_COMPLETE,
  P_PARSING_INCOMPLETE,
  P_PARSING_INVALID
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
  bool finished;

  RequestParser(int fd = -1, size_t max_body_size = 0, size_t buff_max = 200000);
  ~RequestParser();

  void parse();
  Request &get_request();
  bool is_parsing_body() const;
  void reset();

  class InvalidRequestException: public std::exception {
    RequestErrors _error;
  public:
    InvalidRequestException(RequestErrors error);
    const char* what() const throw();
    RequestErrors get_error() const;
  };

  class RequestFinishedException: public std::exception {
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

  size_t content_length;
  size_t max_content_length;
  size_t bytes_consumed;
  bool parsing_body;

  bool chunked;
  size_t chunk_size;
  std::vector<char> chunk_data;

  Request *_request;

  int fd;
  char *buffer;
  size_t bytes_read;
  size_t buff_max;

  std::string _header_key;
  std::string _header_value;

  RequestStates current_state;

  static std::string supported_version;
  size_t supported_version_index;

  ParsingResult tokenize_partial_request(char *buff);
};

#endif // !HTTP_REQUEST_PARSER_HPP
