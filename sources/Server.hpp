//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "Config.hpp"
#include "defines.hpp"

struct server_location {
  std::string root;
  std::vector<std::string> limit_except;
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
  void print(void);
};

#endif  // SERVER_HPP
