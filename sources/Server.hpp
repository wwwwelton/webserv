// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 23/06/2022

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

#include "webserv.h"

struct clientfd {
  int fd;
  int port;
};

class Server {
 public:
  uint16_t port;
  int sockfd;
  std::string root;
  std::string server_name;

  Server(void);
  Server(uint16_t port, int backlog);
  ~Server(void);

  void _socket(void);
  void _bind(uint16_t port);
  void _listen(int backlog);
};

#endif  // SERVER_HPP
