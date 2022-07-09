#ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpBase.hpp"

class Server;

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
typedef void (RequestHandler::*funcptr)(void);
typedef std::map<std::string, void (RequestHandler::*)(void)> meth_map;
private:
  static std::string methods[3];
  static meth_map methodptr;
  static funcptr     fptr[3];
  static meth_map init_map();

  std::string failresponse;
  std::string okresponse;
  std::string httpversion;
  std::string statuscode;
  std::string method;
  Server* server;
  bool    valid;

public:
  RequestHandler(void);
  RequestHandler(Request const& req, Server *_server);

  void _get(void);
  void _post(void);
  void _delete(void);
  void process(void);
  Response _response(void) {
    return (Response());
  }
};

#endif // HTTPHANDLER_HPP
