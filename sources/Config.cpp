// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 25/06/2022

#include "Config.hpp"

Config::Config(void) {
  return;
}

Config::Config(char* file) {
  (void)file;

  std::ifstream ifs;
  std::stringstream ss;
  std::string str;
  std::vector<std::string> configs;

  ifs.open("default.conf");
  ss << ifs.rdbuf();

  str = _sanitize(ss.str());
  configs = _split(str);

  for (size_t i = 0; i < configs.size(); i++) {
    _servers.push_back(_parse(configs[i]));
    std::cout << configs[i] << "\n";
  }

  backlog = DEFAULT_BACKLOG;

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

std::string Config::_sanitize(const std::string& file_content) {
  std::string tmp(file_content);
  size_t pos;

  tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
  tmp.erase(std::remove(tmp.begin(), tmp.end(), '\t'), tmp.end());

  pos = tmp.find("server{");
  while (pos != std::string::npos) {
    tmp.replace(pos, 7, "server {");
    pos += 8;
    pos = tmp.find("server{");
  }

  pos = tmp.find("; ");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, ";");
    pos += 1;
    pos = tmp.find("; ", pos);
  }

  pos = tmp.find("{ ");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, "{");
    pos += 1;
    pos = tmp.find("{ ", pos);
  }

  pos = tmp.find("} ");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, "}");
    pos += 1;
    pos = tmp.find("} ", pos);
  }

  pos = tmp.find(";");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, ";\n");
    pos += 2;
    pos = tmp.find(";", pos);
  }

  pos = tmp.find("{");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, "{\n");
    pos += 2;
    pos = tmp.find("{", pos);
  }

  pos = tmp.find("}");
  while (pos != std::string::npos) {
    tmp.replace(pos, 1, "}\n");
    pos += 2;
    pos = tmp.find("}", pos);
  }

  return (tmp);
}

std::vector<std::string> Config::_split(const std::string& file_content) {
  size_t start, end;
  std::vector<std::string> configs;

  start = file_content.find("server {");
  end = file_content.find("server {", start + 8);
  if (end == std::string::npos) {
    end = file_content.length();
  }

  while (start != std::string::npos) {
    configs.push_back(file_content.substr(start, end - start));
    start += 8;
    start = file_content.find("server {", start);
    end = file_content.find("server {", start + 8);
    if (end == std::string::npos) {
      end = file_content.length();
    }
  }

  return (configs);
}

Server* Config::_parse(const std::string& config) {
  (void)config;
  Server* srv = new Server();

  srv->ip = inet_addr(DEFAULT_ADDRESS);
  srv->port = htons(PORT1);
  srv->server_name.push_back(std::string("www.localhost"));
  srv->server_name.push_back(std::string("localhost"));
  srv->root = std::string(DEFAULT_SERVER_ROOT);
  srv->index.push_back(std::string("index.html"));
  srv->index.push_back(std::string("index2.html"));
  srv->error_page[404] = std::string("custom_404.html");
  srv->error_page[405] = std::string("custom_405.html");
  srv->timeout = DEFAULT_TIMEOUT;
  srv->location["/"].root = std::string(DEFAULT_SERVER_ROOT);
  srv->location["/"].limit_except = std::string(DEFAULT_ROUTE_METHOD);
  srv->location["/"].client_max_body_size = DEFAULT_CLI_MAX_BODY_SIZE;
  srv->location["/"].upload = true;
  srv->location["/"].upload_store = DEFAULT_SERVER_ROOT;
  srv->sockfd = -1;

  return (srv);
}
