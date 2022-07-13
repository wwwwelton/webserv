//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include <arpa/inet.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "webserv.h"

class Server;

class Config {
 public:
  Config(void);
  explicit Config(char* file);
  Config(const Config& src);
  ~Config(void);

  Config& operator=(const Config& rhs);
  Server* operator[](size_t n);

  size_t size(void);

 private:
  std::vector<Server*> _servers;

 private:
  void _replace_all(std::string* str,
                    const std::string& old_word,
                    const std::string& new_word);

  std::string _sanitize(const std::string& file_content);
  std::vector<std::string> _split(const std::string& file_content);
  Server* _parse(const std::string& config);

 public:
  int backlog;
};

#endif  // SERVER_CONFIG_HPP_
