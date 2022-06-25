//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 23/06/2022

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include "webserv.h"

// todo static pollfd

struct clientfd {
  int fd;
  int port;
};

class Server {
public:
  uint16_t        port;
  int             sockfd;
  std::string     root;
  std::string     server_name;
  struct clientfd clients[200];

  Server(void);
  Server(uint16_t port, int backlog);
  ~Server(void);

  void _socket(void);
  void _bind(uint16_t port);
  void _listen(int backlog);
  // virtual void accept_connections(pollfd* pollfd) = 0;
};

#endif  // SERVER_HPP

/*
server {
    listen       80;
    server_name  example.org  www.example.org;
    root         /var/www/port80/
}

server {
    listen       81;
    server_name  example.org  www.example.org;
    root         /var/www/port81/
}
*/
