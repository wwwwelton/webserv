//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#define DFL_TMPFILE "./tmp.html"
#define DFL_DYNFILE "./temp.html"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include<iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "RequestParser.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "ResponseBase.hpp"
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
  static size_t          id;
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

  int           pid;
  int           io[2];
  size_t        thisid;
  std::ifstream file;
  int           postfile;
  std::string   postfilename;

  std::string httpversion;
  std::string statuscode;
  std::string statusmsg;
  std::string contenttype;
  std::string filetype;
  std::string method;
  size_t      body_max_size;

  std::string originalroot;
  std::string path;
  std::string trailing_path;
  std::string url_parameters;
  std::string root;
  std::string bin;

  Server*     server;
  ServerLocation* location;

  bool        folder_request;
  bool        valid;
  bool        remove_tmp;
  bool        header_present;
  bool        response_ready;


  std::string _itoa(size_t nbr);
  int validate_limit_except(void);
  int validate_http_version(void);
  int _get(void);
  int validate_index(void);
  int validate_path(void);
  int validate_folder(void);
  void set_statuscode(int code);
  void cgi(std::string const& body_path, std::string const &bin);
  void dispatch(std::string const& body_path);
  int _post(void);
  void set_environment(void);
  int check_ext(std::string const& body_path);
  int _delete(void);
  int _put(void);
  int _head(void);
  int _connect(void);
  int _options(void);
  int _patch(void);
  int _trace(void);
  void find_location(std::string path, Server *_server);
  void extract_query_parameters(void);
  void create_error_page(void);
  void create_redir_page(void);
  void create_directory_listing(void);

public:
  Request*       req;
  RequestParser* parser;
  bool           finished;
  bool           inprogress;
  bool           incorrect_path;
  bool           path_ends_in_slash;
  std::string    response_path;
  int            response_code;

  ~Response(void);
  Response(Request *req, Server *_server);
  Response(void);
  void assemble_followup(void);
  void assemble_cgi(std::string const& body_path);
  void assemble(std::string const& body_path);
  void assemble(void);
  void set_request(Request* req);
  void reset(void);
  void process(void);
  void _send(int fd);
  std::string get_path(std::string req_path);
  friend std::ostream& operator<<(std::ostream&o, Response const& rhs);
};


#endif // HTTPRESPONSE_HPP
