//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include <arpa/inet.h>

#include <string>
#include <vector>

#include "webserv.hpp"

class Server;

class Config {
 public:
  Config(void);
  explicit Config(char** file);
  Config(const Config& src);
  ~Config(void);

  Config& operator=(const Config& rhs);
  Server* operator[](size_t n);

  size_t size(void);

 private:
  std::vector<Server*> _servers;

 public:
  int backlog;
};

#endif  // SERVER_CONFIG_HPP_
