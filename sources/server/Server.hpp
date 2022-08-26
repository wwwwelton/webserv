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
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "LoadException.hpp"
#include "Logger.hpp"
#include "ServerLocation.hpp"
#include "String.hpp"
#include "defines.hpp"

class ServerLocation;

class Server {
 public:
  in_addr_t ip;
  int port;
  std::vector<std::string> server_name;
  std::string root;
  std::vector<std::string> index;
  std::map<int, std::string> error_page;
  size_t timeout;
  int client_max_body_size;
  std::map<std::string, std::string> log;
  std::map<std::string, std::string> cgi;
  std::pair<int, std::string> redirect;
  std::map<std::string, ServerLocation> location;
  int autoindex;
  int sockfd;
  int upload;
  std::string upload_store;

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
  static void print_addr(std::pair<const int, Server*>& p);

  std::string error;

  class ConnectException : public LoadException {
   public:
    explicit ConnectException(const std::string& str);
    const char* what(void) const throw();
  };

  class BindException : public LoadException {
   public:
    BindException(const std::string& str, int _port, in_addr_t _ip);
    const char* what(void) const throw();
  };
};

#endif  // SERVER_HPP
