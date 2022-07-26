//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Config.hpp"

Config::Config(void) { backlog = DFL_BACKLOG; }

Config::Config(const Config& src) { *this = src; }

Config::~Config(void) {}

Config& Config::operator=(const Config& rhs) {
  if (this != &rhs) {
    backlog = rhs.backlog;
    _servers = rhs._servers;
  }
  return (*this);
}

const Server& Config::operator[](size_t n) { return (_servers[n]); }

size_t Config::size(void) { return (_servers.size()); }

void Config::read(char* file) {
  std::ifstream ifs;
  std::stringstream ss;
  std::string str;
  std::string host;
  std::vector<std::string> vhost;

  ifs.open(file);
  ss << ifs.rdbuf();

  str = _sanitize(ss.str());

  host = _sub_host(str);
  vhost = _sub_vhost(str);

  _parse_host(host);
  _parse_vhost(vhost);
}

std::string Config::_sanitize(const std::string& file_content) {
  std::string tmp(file_content);

  String::replace_all(&tmp, "\n", " ");
  String::replace_all(&tmp, "\t", " ");

  String::replace_all(&tmp, "server{", "server {");

  String::replace_all(&tmp, ";", ";\n");
  String::replace_all(&tmp, "{", "{\n");
  String::replace_all(&tmp, "}", "}\n");

  String::replace_unique(&tmp, ' ');

  String::trim_lines(&tmp, " ");

  return (tmp);
}

std::string Config::_sub_host(const std::string& file_content) {
  size_t pos;
  std::string host;

  pos = file_content.find("server {");
  host = file_content.substr(0, pos);

  return (host);
}

std::vector<std::string> Config::_sub_vhost(const std::string& file_content) {
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

void Config::_parse_host(const std::string& host) {
  std::istringstream is(host);
  std::string line;
  std::vector<std::string> tokens;

  while (std::getline(is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");

    if (tokens[0] == "workers") {
      backlog = String::to_int(tokens[1]);
    }
  }
}

void Config::_parse_vhost(const std::vector<std::string>& vhost) {
  for (size_t i = 0; i < vhost.size(); i++) {
    std::istringstream is(vhost[i]);
    std::string line;
    std::vector<std::string> tokens;

    Server srv;

    while (std::getline(is, line)) {
      line = String::trim(line, "; ");
      tokens = String::split(line, " ");

      if (tokens[0] == "listen") {
        if (tokens[1].find(":") != std::string::npos) {
          tokens = String::split(tokens[1], ":");
          srv.ip = inet_addr(tokens[0].c_str());
          srv.port = htons(String::to_int(tokens[1]));
        } else {
          srv.ip = inet_addr(DFL_ADDRESS);
          srv.port = htons(String::to_int(tokens[1]));
        }
      }

      if (tokens[0] == "server_name") {
        if (srv.server_name[0] == DFL_SERVER_NAME1 &&
            srv.server_name[1] == DFL_SERVER_NAME2) {
          srv.server_name.clear();
        }
        for (size_t i = 1; i < tokens.size(); i++) {
          srv.server_name.push_back(tokens[i]);
        }
      }

      if (tokens[0] == "root") {
        srv.root = String::trim(std::string(tokens[1]), "/");
      }

      if (tokens[0] == "index") {
        if (srv.index[0] == DFL_SERVER_INDEX_PAGE1 &&
            srv.index[1] == DFL_SERVER_INDEX_PAGE2) {
          srv.index.clear();
        }
        for (size_t i = 1; i < tokens.size(); i++) {
          srv.index.push_back(tokens[i]);
        }
      }

      if (tokens[0] == "error_page") {
        srv.error_page[String::to_int(tokens[1])] = String::trim(std::string(tokens[2]), "/");
      }

      if (tokens[0] == "timeout") {
        srv.timeout = String::to_int(tokens[1]);
      }

      if (tokens[0] == "client_max_body_size") {
        srv.client_max_body_size = String::to_int(tokens[1]);
      }

      if (tokens[0] == "access_log") {
        srv.log["access_log"] = tokens[1];
      }

      if (tokens[0] == "error_log") {
        srv.log["error_log"] = tokens[1];
      }

      if (tokens[0] == "autoindex") {
        srv.autoindex = (tokens[1] == "on") ? true : false;
      }

      if (tokens[0] == "cgi") {
        srv.cgi["." + tokens[1]] = tokens[2];
      }

      if (tokens[0] == "return") {
        srv.redirect.first = String::to_int(tokens[1]);
        srv.redirect.second = tokens[2];
      }

      if (tokens[0] == "location") {
        std::string index = tokens[1];

        if (srv.location.find(index) == srv.location.end()) {
          srv.location[index].root = srv.root;
          srv.location[index].index = srv.index;
          srv.location[index].limit_except.push_back(DFL_LIM_EXCEPT);
          srv.location[index].client_max_body_size = srv.client_max_body_size;
          srv.location[index].cgi = srv.cgi;
          srv.location[index].redirect = srv.redirect;
        }

        while (std::getline(is, line)) {
          line = String::trim(line, "; ");
          tokens = String::split(line, " ");

          if (tokens[0] == "root") {
            srv.location[index].root = String::trim(std::string(tokens[1]), "/");
          }

          if (tokens[0] == "index") {
            for (size_t i = 1; i < tokens.size(); i++) {
              srv.location[index].index.push_back(tokens[i]);
            }
          }

          if (tokens[0] == "limit_except") {
            if (srv.location[index].limit_except[0] == DFL_LIM_EXCEPT) {
              srv.location[index].limit_except.pop_back();
            }
            for (size_t i = 1; i < tokens.size(); i++) {
              std::transform(tokens[i].begin(),
                             tokens[i].end(),
                             tokens[i].begin(),
                             ::toupper);
              srv.location[index].limit_except.push_back(tokens[i]);
            }
          }

          if (tokens[0] == "client_max_body_size") {
            srv.location[index].client_max_body_size = String::to_int(tokens[1]);
          }

          if (tokens[0] == "autoindex") {
            srv.location[index].autoindex = (tokens[1] == "on") ? true : false;
          }

          if (tokens[0] == "cgi") {
            srv.location[index].cgi["." + tokens[1]] = tokens[2];
          }

          if (tokens[0] == "return") {
            if (srv.location[index].redirect.first == 0 &&
                srv.location[index].redirect.second.empty()) {
              srv.location[index].redirect.first = String::to_int(tokens[1]);
              srv.location[index].redirect.second = tokens[2];
            }
          }

          if (line == "}") {
            break;
          }
        }
      }
    }

    // srv.print();

    _servers.push_back(srv);
  }
}
