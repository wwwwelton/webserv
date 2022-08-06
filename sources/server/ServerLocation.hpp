//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Server.hpp"
#include "String.hpp"
#include "defines.hpp"

class Server;

class ServerLocation {
 public:
  std::string root;
  std::vector<std::string> index;
  std::vector<std::string> limit_except;
  int client_max_body_size;
  std::map<std::string, std::string> cgi;
  std::pair<int, std::string> redirect;
  int autoindex;
  int upload;
  std::string upload_store;

  ServerLocation(void);
  ServerLocation(const ServerLocation& src);
  ~ServerLocation(void);

  ServerLocation& operator=(const ServerLocation& rhs);

  void fill(const Server& srv);
};

#endif  // SERVERLOCATION_HPP
