#include <string>
#include "webserv.h"

// todo static pollfd

struct clientfd {
  int fd;
  int port;
};

class server {
public:
  unsigned short port;
  int           socketfd;
  std::string   root;
  std::string   server_name;
  struct clientfd  clients[200];

  virtual void accept_connections(pollfd *pollfd) = 0;
};

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
