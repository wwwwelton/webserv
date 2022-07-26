//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Server.hpp"
#include "String.hpp"

class Server;

class Config {
 public:
  Config(void);
  Config(const Config& src);
  ~Config(void);

  Config& operator=(const Config& rhs);
  const Server& operator[](size_t n);

  size_t size(void);

  void read(char* file);

 private:
  std::string _sanitize(const std::string& file_content);
  std::string _sub_host(const std::string& file_content);
  std::vector<std::string> _sub_vhost(const std::string& file_content);

  void _parse_host(const std::string& host);
  void _parse_vhost(const std::vector<std::string>& vhost);

 public:
  int backlog;

 private:
  std::vector<Server> _servers;
};

#endif  // CONFIG_HPP_
