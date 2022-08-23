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
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ConfigHelper.hpp"
#include "LoadException.hpp"
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
  void load(char* file);

 private:
  std::string _open(char* file);
  std::string _sanitize(const std::string& file_content);
  void _purge_comments(std::string* str);
  bool _is_valid(const std::string& file_content);
  std::vector<std::string> _split_line(std::string line);
  void _parse(std::istringstream* is);
  Server _parse_server(std::istringstream* is);
  ServerLocation _parse_location(std::istringstream* is);

 public:
  int backlog;
  std::set<std::string> cgi_list;

 private:
  std::vector<Server> _servers;
};

#endif  // CONFIG_HPP_
