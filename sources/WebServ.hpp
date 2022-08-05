//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Config.hpp"
#include "ResponseBase.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "LoadException.hpp"
#include "Logger.hpp"
#include "Pollfd.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"
#include "ServerLocation.hpp"
#include "String.hpp"
#include "defines.hpp"
#include "signal.hpp"
#include "validate_input.hpp"

class Request;
class Response;
typedef struct addrinfo s_addrinfo;

typedef struct s_request {
  s_request();
  s_request(Server *_server, int fd);
  Server *server;
  RequestParser *request_parser;
  Response *response;
  size_t timestamp;
} req;

class WebServ {
 public:
  static size_t get_time_in_ms(void);

  WebServ(void);
  ~WebServ(void);
  void init(int argc, char **argv);
  int _poll(void);
  void _accept(int fd);
  void _receive(int fd);
  void _respond(int fd);
  void end_connection(int fd);
  void purge_conns(void);
  void purge_timeouts(void);
  bool timed_out(int fd);
  static Logger init_log(void);
  void init_servers(void);

 public:
  Config conf;
  std::map<int, Server *> serverlist;
  std::vector<req> clientlist;
  std::vector<_pollfd> pollfds;
  static Logger log;
  int conn;
  int compress;
};

#endif  // WEBSERV_HPP
