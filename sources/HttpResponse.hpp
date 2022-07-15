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

class Response {
typedef void(Response::*funcptr)(void);
typedef std::map<std::string, int (Response::*)(void)> meth_map;
typedef std::vector<int (Response::*)(void)> function_vector;
typedef std::map<int, std::string> status_map;

private:
  static std::map<int, std::string> statuslist;
  static meth_map methodptr;
  static function_vector pre_method;
  static function_vector get_method;

  static meth_map        init_map();
  static function_vector init_pre();
  static function_vector init_get();
  static status_map      init_status_map();

  std::string response_path;
  int         response_code;

  std::string httpversion;
  std::string statuscode;
  std::string statusmsg;
  std::string contenttype;
  std::string method;
  std::string req_body;
  std::string path;
  std::string root;
  Server*     server;
  Request const* req;
  bool        valid;
  server_location* location;

  int validate_limit_except(void);
  int validate_http_version(void);
  int _get(void);
  int validate_index(void);
  int validate_path(void);
  void set_statuscode(int code);
  void assemble(std::string const& body_path);
  void php_cgi(std::string const& body_path);
  void dispatch(std::string const& body_path);
  int _post(void);
  int _delete(void);
  void find_location(std::string path, Server *_server);

public:
  Response(Request const& req, Server *_server);
  Response(void);
  void process(void);
  void _send(int fd);
};

#endif // HTTPRESPONSE_HPP
