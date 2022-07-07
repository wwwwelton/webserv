#pragma once
#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include "HttpRequest.hpp"
#include "HttpBase.hpp"

#include <sys/socket.h>
#include <exception>
#include <map>
#include <string>
#include <cstring>

struct Request {
  std::map<std::string, std::string> headers;
  std::string method;
  std::string path;
  std::string http_version;
  std::string host;
  std::string body;
  int body_size;

  Request(int fd);

  ~Request();

private:
  char *raw;

};

#endif // !HTTP_REQUEST_HPP
