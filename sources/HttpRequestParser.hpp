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
  S_HTTP_VERSION_H,
  S_HTTP_VERSION_HT,
  S_HTTP_VERSION_HTT,
  S_HTTP_VERSION_HTTP,
  S_HTTP_VERSION_SLASH,
  S_HTTP_VERSION_MAJOR,
  S_HTTP_VERSION_POINT,
  S_HTTP_VERSION_MINOR,

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

  S_DEAD = 0
};

enum TokenType {
  TK_WORD,
  TK_NEWLINE,
  TK_COLON
};

class HttpRequestParser
{
public:
  bool finished;

  class InvalidHttpRequestException: public std::exception {
  public:
    const char* what() const throw() { return "invalid http request"; }
  };

  HttpRequestParser(int fd = -1, size_t buff_max = 512);
  HttpRequestParser(const HttpRequestParser &);
  HttpRequestParser &operator=(const HttpRequestParser &);
  ~HttpRequestParser();

  struct Token {
    const char *value;
    const size_t size;
    const TokenType type;
      
    Token(const char *value, size_t size, TokenType type);
  };

  void parse();
  Request &get_request();

private:
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

  void tokenize_partial_request(char *buff);
};

#endif // !HTTP_REQUEST_PARSER_HPP
