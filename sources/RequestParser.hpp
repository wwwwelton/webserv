#pragma once
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "HttpRequest.hpp"
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
  S_BODY_CRLF,
  S_BODY_CR,
  S_BODY_LF,

  S_CHUNK_START,
  S_CHUNK_SIZE,
  S_CHUNK_EXTENSIONS,
  S_CHUNK_SIZE_LF,
  S_CHUNK_DATA_START,
  S_CHUNK_DATA,
  S_CHUNK_DATA_CRLF,
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

enum TokenType {
  TK_WORD,
  TK_NEWLINE,
  TK_COLON
};

class RequestParser
{
  RequestParser(const RequestParser &);
  RequestParser &operator=(const RequestParser &);
public:
  bool finished;

  class InvalidHttpRequestException: public std::exception {
  public:
    const char* what() const throw() { return "invalid http request"; }
  };

  RequestParser(int fd = -1, size_t buff_max = 2000);
  ~RequestParser();

  struct Token {
    const char *value;
    const size_t size;
    const TokenType type;
      
    Token(const char *value, size_t size, TokenType type);
  };

  void parse();
  Request &get_request();

private:
  size_t content_length;
  bool chunked;
  size_t chunk_size;
  std::vector<char> chunk_data;
  bool valid;
  Request *_request;
  std::vector<Token> headers;

  int fd;
  char *buffer;
  size_t bytes_read;
  size_t buff_max;

  std::string _header_key;
  std::string _header_value;
  std::vector<Token> tokens;

  RequestStates current_state;

  static std::string supported_version;
  size_t supported_version_index;

  ParsingResult tokenize_partial_request(char *buff);
};

#endif // !HTTP_REQUEST_PARSER_HPP
