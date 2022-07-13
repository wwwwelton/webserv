#pragma once
#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include "HttpRequest.hpp"
#include "HttpBase.hpp"

#include <map>
#include <string>
#include <vector>

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

  bool is_valid() const ;

  Request* receive(void);
private:
  int fd;
  bool valid;
  char *raw;
  size_t nbytes;

  std::vector<std::string>* tokenize_request(char *payload);
  void parse_request(std::vector<std::string>* tokens);

};
std::ostream& operator<<(std::ostream& out, const Request& request);
#endif // !HTTP_REQUEST_HPP
