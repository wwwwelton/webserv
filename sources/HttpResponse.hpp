//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#define DFL_TMPFILE "./tmp.html"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpBase.hpp"
#include "WebServ.hpp"
#include "Logger.hpp"

class Server;
class Logger;
class Request;


class Response {
typedef void(Response::*funcptr)(void);
typedef std::map<std::string, int (Response::*)(void)>    meth_map;
typedef std::vector<int (Response::*)(void)>              function_vector;
typedef std::map<int, std::string>                        status_map;
typedef std::map<std::string, std::string>                mimetypes_map;

private:
  static status_map      statuslist;
  static mimetypes_map   mimetypes;
  static meth_map        method_map;
  static function_vector validation_functions;
  static function_vector get_functions;

  static meth_map        init_map();
  static function_vector init_pre();
  static function_vector init_get();
  static status_map      init_status_map();
  static mimetypes_map   init_mimetypes();

  int           response_code;
  std::ifstream file;

  std::string httpversion;
  std::string statuscode;
  std::string statusmsg;
  std::string contenttype;
  std::string filetype;
  std::string method;
  size_t      body_max_size;

  Request const* req;
  std::string req_body;
  std::string path;
  std::string root;

  Server*     server;
  ServerLocation* location;

  bool        folder_request;
  bool        valid;
  bool        remove_tmp;

  std::string _itoa(size_t nbr);
  int validate_limit_except(void);
  int validate_http_version(void);
  int _get(void);
  int validate_index(void);
  int validate_path(void);
  int validate_folder(void);
  void set_statuscode(int code);
  void php_cgi(std::string const& body_path);
  void dispatch(std::string const& body_path);
  int _post(void);
  int _delete(void);
  void find_location(std::string path, Server *_server);
  void create_error_page(void);
  void create_redir_page(void);
  void create_directory_listing(void);

public:
  bool          finished;
  bool          inprogress;
  std::string   response_path;

  ~Response(void);
  Response(Request *req, Server *_server);
  Response(void);
  void assemble_followup(void);
  void assemble(std::string const& body_path);
  void set_request(Request const* req);
  void process(void);
  void _send(int fd);
};

#endif // HTTPRESPONSE_HPP
