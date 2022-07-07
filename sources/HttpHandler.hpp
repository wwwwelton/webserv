#ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "HttpRequest.hpp"
#include "HttpBase.hpp"

struct Response {
  std::map<std::string, std::string> headers;
  char *path;
  char *http_version;
  int status_code;
  char *status_code_description;
  char *host;

  void _send(int fd);
};

#define BUFFER_SIZE 100

class RequestHandler {
public:
  RequestHandler(void);
  RequestHandler(Request const& req);

  Response _response(void) {
    return (Response());
  }
};

#endif // HTTPHANDLER_HPP
