//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

#include "Config.hpp"

Config::Config(void) {
  return;
}

Config::Config(char* file) {
  (void)file;

  std::ifstream ifs;
  std::stringstream ss;
  std::string str;
  std::string host;
  std::vector<std::string> vhost;

  ifs.open("default.conf");
  ss << ifs.rdbuf();

  str = _sanitize(ss.str());
  host = _sub_host(str);
  vhost = _split(str);

  std::cout << host << "\n";
  for (size_t i = 0; i < vhost.size(); i++) {
    _servers.push_back(_parse(vhost[i]));
    std::cout << vhost[i] << "\n";
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

void Config::_replace_all(std::string* str,
                          const std::string& old_word,
                          const std::string& new_word) {
  size_t pos;

  pos = str->find(old_word);
  while (pos != std::string::npos) {
    str->replace(pos, old_word.length(), new_word);
    pos = str->find(old_word, pos + new_word.length());
  }
}

std::string Config::_sanitize(const std::string& file_content) {
  std::string tmp(file_content);

  tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
  tmp.erase(std::remove(tmp.begin(), tmp.end(), '\t'), tmp.end());

  _replace_all(&tmp, "server{", "server {");

  _replace_all(&tmp, "; ", ";");
  _replace_all(&tmp, "{ ", "{");
  _replace_all(&tmp, "} ", "}");

  _replace_all(&tmp, ";", ";\n");
  _replace_all(&tmp, "{", "{\n");
  _replace_all(&tmp, "}", "}\n");

  return (tmp);
}

std::string Config::_sub_host(const std::string& file_content) {
  size_t pos;
  std::string host;

  pos = file_content.find("server {");
  host = file_content.substr(0, pos);

  return (host);
}

std::vector<std::string> Config::_split(const std::string& file_content) {
  size_t start, end;
  std::vector<std::string> vhost;

  start = file_content.find("server {");
  end = file_content.find("server {", start + 8);
  if (end == std::string::npos) {
    end = file_content.length();
  }

  while (start != std::string::npos) {
    vhost.push_back(file_content.substr(start, end - start));
    start += 8;
    start = file_content.find("server {", start);
    end = file_content.find("server {", start + 8);
    if (end == std::string::npos) {
      end = file_content.length();
    }
  }

  return (vhost);
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
