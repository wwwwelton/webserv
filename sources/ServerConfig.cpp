// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 25/06/2022

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void) {
  return;
}

ServerConfig::ServerConfig(char** file) {
  (void)file;
  _servers.push_back(server_config());
  _servers[0].listen = PORT1;
  _servers[0].root = "/";
  _servers[0].server_name = "Server1";

  _servers.push_back(server_config());
  _servers[1].listen = PORT2;
  _servers[1].root = "/";
  _servers[1].server_name = "Server2";

  return;
}

ServerConfig::ServerConfig(const ServerConfig& src) {
  *this = src;
}

ServerConfig::~ServerConfig(void) {
  return;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs) {
  this->_servers = rhs._servers;
  return (*this);
}

server_config& ServerConfig::operator[](size_t n) {
  return (_servers[n]);
}

size_t ServerConfig::size(void) {
  return (_servers.size());
}
