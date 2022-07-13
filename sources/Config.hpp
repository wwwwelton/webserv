// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 25/06/2022

#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include <arpa/inet.h>

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
  std::string _serialize(const std::string& file_content);
  std::string _prepare(const std::string& file_content);
  std::vector<std::string> _split(const std::string& file_content);
  Server* _parse(const std::string& config);

 public:
  int backlog;
};

#endif  // SERVER_CONFIG_HPP_
