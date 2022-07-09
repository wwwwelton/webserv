// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 24/06/2022

#include "Server.hpp"

Server::Server(void) {
  ip = htonl(INADDR_LOOPBACK);
  port = 0;
  server_name = std::vector<std::string>();
  root = std::string("");
  index = std::vector<std::string>();
  error_page = std::map<int, std::string>();
  timeout = DEFAULT_TIMEOUT;
  location = std::map<std::string, server_location>();
  sockfd = -1;
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

void Server::_bind(void) {
  struct sockaddr_in sockaddress;

  sockaddress.sin_family = AF_INET;
  sockaddress.sin_port = port;
  sockaddress.sin_addr.s_addr = ip;

  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)) {
    perror("setsockopt");
    exit(42);
  }
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

int Server::_connect(int backlog) {
  _socket();
  _bind();
  _listen(backlog);
  return 0;
}
