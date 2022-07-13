// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 23/06/2022

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netdb.h>

#include <map>
#include <string>
#include <vector>

#include "webserv.hpp"

struct server_location {
  std::string root;
  std::string limit_except;
  int client_max_body_size;
  bool upload;
  std::string upload_store;
};

class Server {
 public:
  in_addr_t ip;
  int port;
  std::vector<std::string> server_name;
  std::string root;
  std::vector<std::string> index;
  std::map<int, std::string> error_page;
  int timeout;
  std::map<std::string, server_location> location;
  int sockfd;

  Server(void);
  ~Server(void);

  void _socket(void);
  void _bind(void);
  void _listen(int backlog);
  int _connect(int backlog);
};

#endif  // SERVER_HPP
