// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 25/06/2022

#include "Config.hpp"

Config::Config(void) {
  return;
}

Config::Config(char** file) {
  (void)file;

  backlog = DEFAULT_BACKLOG;

  _servers.push_back(new Server());

  _servers[0]->ip = htonl(INADDR_LOOPBACK);
  _servers[0]->port = PORT1;
  _servers[0]->server_name.push_back(std::string("www.localhost"));
  _servers[0]->server_name.push_back(std::string("localhost"));
  _servers[0]->root = std::string(DEFAULT_SERVER_ROOT);
  _servers[0]->index.push_back(std::string("index.html"));
  _servers[0]->index.push_back(std::string("index2.html"));
  _servers[0]->error_page[404] = std::string("custom_404.html");
  _servers[0]->error_page[405] = std::string("custom_405.html");
  _servers[0]->timeout = DEFAULT_TIMEOUT;
  _servers[0]->location["/"].root = std::string(DEFAULT_SERVER_ROOT);
  _servers[0]->location["/"].limit_except = std::string(DEFAULT_ROUTE_METHOD);
  _servers[0]->location["/"].client_max_body_size = DEFAULT_CLI_MAX_BODY_SIZE;
  _servers[0]->location["/"].upload = true;
  _servers[0]->location["/"].upload_store = DEFAULT_SERVER_ROOT;
  _servers[0]->sockfd = -1;

  _servers.push_back(new Server());

  _servers[1]->ip = htonl(INADDR_LOOPBACK);
  _servers[1]->port = PORT2;
  _servers[1]->server_name.push_back(std::string("www.localhost2"));
  _servers[1]->server_name.push_back(std::string("localhost2"));
  _servers[1]->root = std::string(DEFAULT_SERVER_ROOT);
  _servers[1]->index.push_back(std::string("index.html"));
  _servers[1]->index.push_back(std::string("index2.html"));
  _servers[1]->error_page[404] = std::string("custom_404.html");
  _servers[1]->error_page[405] = std::string("custom_405.html");
  _servers[1]->timeout = DEFAULT_TIMEOUT;
  _servers[1]->location["/"].root = std::string(DEFAULT_SERVER_ROOT);
  _servers[1]->location["/"].limit_except = std::string(DEFAULT_ROUTE_METHOD);
  _servers[1]->location["/"].client_max_body_size = DEFAULT_CLI_MAX_BODY_SIZE;
  _servers[1]->location["/"].upload = true;
  _servers[1]->location["/"].upload_store = DEFAULT_SERVER_ROOT;
  _servers[1]->sockfd = -1;

  return;
}

Config::Config(const Config& src) {
  *this = src;
}

Config::~Config(void) {
  return;
}

Config& Config::operator=(const Config& rhs) {
  this->_servers = rhs._servers;
  return (*this);
}

Server* Config::operator[](size_t n) {
  return (_servers[n]);
}

size_t Config::size(void) {
  return (_servers.size());
}
