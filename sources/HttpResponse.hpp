//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#define BUFFER_SIZE 100

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpBase.hpp"
#include "Logger.hpp"

class Server;
class Logger;
class server_location;

// struct Response {
//   std::map<std::string, std::string> headers;
//   char *path;
//   char *http_version;
//   int status_code;
//   char *status_code_description;
//   char *host;

//   void _send(int fd);
// };


class Response {
typedef void(Response::*funcptr)(void);
typedef std::map<std::string, int (Response::*)(void)> meth_map;
typedef std::vector<int (Response::*)(void)> function_vector;

private:
  static meth_map methodptr;
  static function_vector pre_method;

  std::string response_path;
  int         response_code;

  std::string httpversion;
  std::string statuscode;
  std::string statusmsg;
  std::string method;
  std::string path;
  std::string root;
  Server*     server;
  bool        valid;
  server_location* location;

  static meth_map init_map();
  static function_vector init_pre();
  static Logger init_logger();


  int _get(void);
  int validate_limit_except(void);
  void _get_body(std::string const& body_path);
  void _get_php_cgi(std::string const& body_path);
  void extension_dispatcher(std::string const& body_path);
  int _post(void);
  int _delete(void);
  void find_location(std::string path, Server *_server);

public:
  Response(Request const& req, Server *_server);
  Response(void);
  void process(void);
  void _send(int fd);
  // Response _response(void) {
  //   return (Response());
  // }
};

#endif // HTTPRESPONSE_HPP
