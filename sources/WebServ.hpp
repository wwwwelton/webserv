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
#include "HttpBase.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "LoadException.hpp"
#include "Logger.hpp"
#include "Pollfd.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"
#include "ServerLocation.hpp"
#include "defines.hpp"

class Request;
class Response;
typedef struct addrinfo s_addrinfo;

typedef struct s_request {
  s_request();
  s_request(Server *_server, int fd);
  Server *server;
  RequestParser *request_parser;
  Response *response;
} req;

class WebServ {
 public:
  WebServ(int argc, char **argv);
  ~WebServ();
  int _poll(void);
  void _accept(int fd);
  void _receive(int fd);
  void _respond(int fd);
  void end_connection(int fd);
  void purge_conns(void);
  static Logger init_log(void);

 public:
  Config conf;
  std::map<int, Server *> serverlist;
  std::vector<req> clientlist;
  std::vector<_pollfd> pollfds;
  static Logger log;
  int conn;
  int compress;

 private:
  void _validate_input(int argc, char **argv);
  void _init_signals(void);
};

void sighandler(const int signal, void *ptr);

#endif  // WEBSERV_HPP
