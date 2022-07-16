// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 24/06/2022

#include "Server.hpp"

Server::Server(void) {
  return;
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
  (void)backlog;
  if (listen(sockfd, 500)) {
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

void Server::print(void) {
  static size_t n = 1;

  std::cout << "====VHOST " << n << "====\n";

  in_addr t;
  t.s_addr = ip;
  std::cout << "ip: =>" << inet_ntoa(t) << "<=\n";

  std::cout << "port: =>" << ntohs(port) << "<=\n";

  for (size_t i = 0; i < server_name.size(); i++) {
    std::cout << "server name: =>" << server_name[i] << "<=\n";
  }

  std::cout << "root: =>" << root << "<=\n";

  for (size_t i = 0; i < index.size(); i++) {
    std::cout << "index: =>" << index[i] << "<=\n";
  }

  for (std::map<int, std::string>::const_iterator it = error_page.begin();
       it != error_page.end();
       it++) {
    std::cout << "error_page: =>" << it->second << "<=\n";
  }

  std::cout << "timeout: =>" << timeout << "<=\n";

  for (std::map<std::string, server_location>::const_iterator
           it = location.begin();
       it != location.end();
       it++) {
    std::cout << "location name: =>" << it->first << "<=\n";
    std::cout << "    root: =>" << it->second.root << "<=\n";
    std::cout << "    limit_except: =>" << it->second.limit_except << "<=\n";
    std::cout << "    client_max_body_size: =>" << it->second.client_max_body_size << "<=\n";
    std::cout << "    upload: =>" << it->second.upload << "<=\n";
    std::cout << "    upload_store: =>" << it->second.upload_store << "<=\n";
  }

  std::cout << "sockfd: =>" << sockfd << "<=\n";

  std::cout << "\n";
  n++;
}
