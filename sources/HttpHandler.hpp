#ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpBase.hpp"
#include "Logger.hpp"

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
  static meth_map methodptr;

  std::string httpversion;
  std::string statuscode;
  std::string statusmsg;
  std::string location;
  std::string method;
  std::string path;
  std::string root;
  Server*     server;
  bool        valid;

  static meth_map init_map();
  void _get(void);
  void _get_body(std::string const& body_path);
  void _get_php_cgi(std::string const& body_path);
  void extension_dispatcher(std::string const& body_path);
  void _post(void);
  void _delete(void);
  std::string find_location(std::string path, Server *_server);

public:
  RequestHandler(void);
  RequestHandler(Request const& req, Server *_server);
  void process(void);
  Response _response(void) {
    return (Response());
  }
};

#endif // HTTPHANDLER_HPP
