//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

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
class Logger;

// struct Response {
//   std::map<std::string, std::string> headers;
//   char *path;
//   char *http_version;
//   int status_code;
//   char *status_code_description;
//   char *host;

//   void _send(int fd);
// };

#define BUFFER_SIZE 100

class Response {
typedef void (Response::*funcptr)(void);
typedef std::map<std::string, void (Response::*)(void)> meth_map;

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
  static Logger init_logger();
  void _get(void);
  void _get_body(std::string const& body_path);
  void _get_php_cgi(std::string const& body_path);
  void extension_dispatcher(std::string const& body_path);
  void _post(void);
  void _delete(void);
  std::string find_location(std::string path, Server *_server);

public:
  Response(void);
  Response(Request const& req, Server *_server);
  void process(void);
  void _send(int fd);
  // Response _response(void) {
  //   return (Response());
  // }
};

#endif // HTTPRESPONSE_HPP
