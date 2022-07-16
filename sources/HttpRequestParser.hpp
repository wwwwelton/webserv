#pragma once
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "HttpRequest.hpp"
#include <exception>
#include <string>
#include <vector>

enum RequestStates {
  S_INIT = 1,

  S_METHOD,
  S_PATH,
  S_PROTOCOL, // first line

  S_HEADERS,
  S_BODY,

  S_DEAD
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

  class StateNotImplementedException: public std::exception {
  public:
    const char* what() const throw() { return "reached an unimplemented state"; }
  };

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

  void tokenize_partial_request();
  Request *get_request();

private:
  int fd;
  char *buffer;
  size_t i;
  size_t bytes_read;
  size_t buff_max;

  RequestStates current_state;
};

#endif // !HTTP_REQUEST_PARSER_HPP
