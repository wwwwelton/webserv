// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 25/06/2022

#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include <string>
#include <vector>

#include "webserv.h"

struct server_config {
  int listen;
  std::string server_name;
  std::string root;
};

class ServerConfig {
 public:
  explicit ServerConfig(char** file);
  ServerConfig(const ServerConfig& src);
  ~ServerConfig(void);

  ServerConfig& operator=(const ServerConfig& rhs);
  server_config& operator[](size_t n);

  size_t size(void);

 private:
  ServerConfig(void);
  std::vector<struct server_config> _servers;
};

#endif  // SERVER_CONFIG_HPP_
