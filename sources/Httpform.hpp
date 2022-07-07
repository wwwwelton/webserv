#ifndef HTTPFORM_HPP
#define HTTPFORM_HPP

#include "HttpRequest.hpp"

#include <sys/socket.h>
#include <sys/types.h>

#include <map>
#include <string>

struct Response {
  std::map<std::string, std::string> headers;
  char *path;
  char *http_version;
  int status_code;
  char *status_code_description;
  char *host;

  void _send(int fd) {
    send(fd, HttpBase::buffer_resp, 74, 0);
  }
};

struct RequestHandler {
  RequestHandler(void) {
    return;
  }

  RequestHandler(Request& req) {
    (void)req;
    return;
  }

  Response _response(void) {
    return (Response());
  }
};

#endif  // HTTPFORM_HPP
