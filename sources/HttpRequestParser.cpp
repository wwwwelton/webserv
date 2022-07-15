#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"
#include <cstddef>
#include <exception>

HttpRequestParser::HttpRequestParser(int fd, size_t buff_max):
  finished(false),
  fd(fd),
  buffer(new char[buff_max]),
  i(),
  bytes_read(),
  buff_max(buff_max),
  current_state(S_INIT) { }

HttpRequestParser::HttpRequestParser(const HttpRequestParser &other):
  finished(false),
  fd(other.fd),
  buffer(new char[other.buff_max]),
  i(other.i),
  bytes_read(other.bytes_read),
  buff_max(other.buff_max),
  current_state(other.current_state) { }

HttpRequestParser& HttpRequestParser::operator=(const HttpRequestParser &other) {
  delete[] buffer;
  buffer = new char[other.buff_max];
  buff_max = other.buff_max;
  i = other.i;
  bytes_read = other.bytes_read;
  finished = other.finished;
  return *this;
}

HttpRequestParser::~HttpRequestParser() {
  delete[] buffer;
}

void HttpRequestParser::tokenize_partial_request() {
  switch (current_state) {
  case S_INIT:
    current_state = S_METHOD;
    break ;
  default:
    current_state = S_DEAD;
    throw StateNotImplementedException();
  }
  finished = true;
  return ;
}

Request *HttpRequestParser::get_request() {
  Request * request = new Request();
  request->method = "GET";
  request->path = "/index.html";
  request->http_version = "HTTP/1.1";
  request->host = "localhost:3492";
  request->finished = true;
  request->valid = true;
  return request;
}

HttpRequestParser::Token::Token(const char *value, size_t size, TokenType type)
  : value(value), size(size), type(type) { }
