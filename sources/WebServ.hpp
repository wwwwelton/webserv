//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define PORT1 3492
#define PORT2 3493

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "HttpBase.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Pollfd.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "defines.hpp"
#include "Logger.hpp"

class Request;
typedef struct addrinfo s_addrinfo;

typedef struct s_request {
  s_request();
  s_request(Server *_server, int fd);
  Server  *server;
  Request *request;
} req;

class WebServ {
public:
  WebServ(int argc, char **argv);
  int _poll(void);
  void _accept(int fd);
  void _respond(int fd);
  void purge_conns(void);
  static Logger init_log();

public:
  Config                  configs;
  std::map<int, Server *> serverlist;
  std::vector<req>        clientlist;
  std::vector<_pollfd>    pollfds;
  static Logger           log;
  int                     conn;
  int                     compress;
};

#endif  // WEBSERV_HPP
