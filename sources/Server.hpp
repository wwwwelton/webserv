//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Config.hpp"
#include "defines.hpp"

struct server_location {
  std::string root;
  std::vector<std::string> index;
  std::vector<std::string> limit_except;
  int client_max_body_size;
  // extension | cgi binary i.e.: .php | php-cgi
  std::map<std::string, std::string> cgi;
  // if int = true: return code | url to redirect to
  std::pair<int, std::string> redirect;
  bool autoindex;
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
  int client_max_body_size;
  std::map<std::string, std::string> log;
  std::map<std::string, std::string> cgi;
  std::pair<int, std::string> redirect;
  std::map<std::string, server_location> location;
  bool autoindex;
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
