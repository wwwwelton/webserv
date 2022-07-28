//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ServerLocation.hpp"
#include "String.hpp"
#include "defines.hpp"

class Server {
 public:
  in_addr_t ip;
  int port;
  std::vector<std::string> server_name;
  std::string root;
  std::vector<std::string> index;
  std::map<int, std::string> error_page;
  int timeout;
  int client_max_body_size;
  std::map<std::string, std::string> log;
  std::map<std::string, std::string> cgi;
  std::pair<int, std::string> redirect;
  std::map<std::string, ServerLocation> location;
  int autoindex;
  int sockfd;

  Server(void);
  Server(const Server& src);
  ~Server(void);

  Server& operator=(const Server& rhs);

  void fill(void);
  bool is_invalid(void);
  void _socket(void);
  void _bind(void);
  void _listen(int backlog);
  int _connect(int backlog);
  void print(void);
};

#endif  // SERVER_HPP
