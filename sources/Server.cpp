//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 24/06/2022

#include "Server.hpp"
#include <netdb.h>

#define PORT 3490

Server::Server(void) {
  _socket();
}

Server::Server(uint16_t _port, int backlog) {
  this->port = _port;
  _socket();
  _bind(port);
  _listen(backlog);
}

Server::~Server(void) {
  close(sockfd);
}

void Server::_socket(void) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    exit(errno);
  }
  fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

void Server::_bind(uint16_t port) {
  struct sockaddr_in sockaddress;

  sockaddress.sin_family = AF_INET;
  sockaddress.sin_port = htons(port);
  sockaddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (bind(sockfd, (const sockaddr*)&sockaddress, sizeof(sockaddr_in))) {
    perror("bind");
    close(sockfd);
    exit(errno);
  }
}

void Server::_listen(int backlog) {
  if (listen(sockfd, backlog)) {
    perror("listen");
    exit(errno);
  }
}
