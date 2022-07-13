//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
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
#include "HttpHandler.hpp"
#include "HttpRequest.hpp"
#include "Pollfd.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "defines.hpp"

class Server;
typedef struct addrinfo s_addrinfo;

typedef struct s_request {
  s_request()
    : server(NULL), request(NULL) { }
  s_request(Server *_server, int fd)
    : server(_server), request(new Request(fd)) { }
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

public:
  std::map<int, Server *> serverlist;
  std::vector<req>        clientlist;
  std::vector<_pollfd>    pollfds;
  int                     conn;
  int                     compress;
};

#endif  // WEBSERV_HPP
