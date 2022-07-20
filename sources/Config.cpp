//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Config.hpp"

Config::Config(void) {
  return;
}

Config::Config(char* file) {
  std::ifstream ifs;
  std::stringstream ss;
  std::string str;
  std::string host;
  std::vector<std::string> vhost;

  ifs.open(file);
  ss << ifs.rdbuf();

  str = _sanitize(ss.str());

  // std::cout << str << "\n";

  host = _sub_host(str);
  vhost = _sub_vhost(str);

  _parse_host(host);

  //   std::cout << host << "\n";
  for (size_t i = 0; i < vhost.size(); i++) {
    _servers.push_back(_parse_vhost(vhost[i]));
    // std::cout << vhost[i] << "\n";
  }

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

  _replace_all(&tmp, "\n", " ");
  _replace_all(&tmp, "\t", " ");

  _replace_all(&tmp, "server{", "server {");

  _replace_all(&tmp, ";", ";\n");
  _replace_all(&tmp, "{", "{\n");
  _replace_all(&tmp, "}", "}\n");

  _replace_unique(&tmp, ' ');

  _trim_line(&tmp, " ");

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
    line = _trim(line, "; ");
    tokens = _split(line, " ");

    if (tokens[0] == "workers") {
      backlog = _stoi(tokens[1]);
    }
  }
}

Server* Config::_parse_vhost(const std::string& vhost) {
  std::istringstream is(vhost);
  std::string line;
  std::vector<std::string> tokens;
  std::vector<std::string> sub_tokens;

  Server* srv = new Server();

  srv->client_max_body_size = DFL_CLI_MAX_BODY_SIZE;
  srv->autoindex = false;

  while (std::getline(is, line)) {
    line = _trim(line, "; ");
    tokens = _split(line, " ");

    if (tokens[0] == "listen") {
      if (tokens[1].find(":") != std::string::npos) {
        sub_tokens = _split(tokens[1], ":");
        srv->ip = inet_addr(sub_tokens[0].c_str());
        srv->port = htons(_stoi(sub_tokens[1]));
      } else {
        srv->ip = inet_addr(DFL_ADDRESS);
        srv->port = htons(_stoi(tokens[1]));
      }
    }

    if (tokens[0] == "server_name") {
      for (size_t i = 1; i < tokens.size(); i++) {
        srv->server_name.push_back(tokens[i]);
      }
    }

    if (tokens[0] == "root") {
      srv->root = _trim(std::string(tokens[1]), "/");
    }

    if (tokens[0] == "index") {
      for (size_t i = 1; i < tokens.size(); i++) {
        srv->index.push_back(tokens[i]);
      }
    }

    if (tokens[0] == "error_page") {
      srv->error_page[_stoi(tokens[1])] = _trim(std::string(tokens[2]), "/");
    }

    if (tokens[0] == "timeout") {
      srv->timeout = _stoi(tokens[1]);
    }

    if (tokens[0] == "client_max_body_size") {
      srv->client_max_body_size = _stoi(tokens[1]);
    }

    if (tokens[0] == "access_log") {
      srv->log["access_log"] = tokens[1];
    }

    if (tokens[0] == "error_log") {
      srv->log["error_log"] = tokens[1];
    }

    if (tokens[0] == "autoindex") {
      srv->autoindex = (tokens[1] == "on") ? true : false;
    }

    if (tokens[0] == "cgi") {
      srv->cgi["." + tokens[1]] = tokens[2];
    }

    if (tokens[0] == "location") {
      std::string index = tokens[1];

      if (srv->location.find(index) == srv->location.end()) {
        srv->location[index].root = srv->root;
        srv->location[index].limit_except.push_back(DFL_LIM_EXCEPT);
        srv->location[index].client_max_body_size = srv->client_max_body_size;
        srv->location[index].log = srv->log;
        srv->location[index].cgi = srv->cgi;

        // TODO(wleite): remove
        srv->location[index].upload = false;
        srv->location[index].upload_store = srv->root + "/uploads";
      }

      while (std::getline(is, line)) {
        line = _trim(line, "; ");
        tokens = _split(line, " ");

        if (tokens[0] == "root") {
          srv->location[index].root = tokens[1];
          srv->location[index].root = _trim(std::string(tokens[1]), "/");
        }
        if (tokens[0] == "limit_except") {
          if (srv->location[index].limit_except[0] == DFL_LIM_EXCEPT) {
            srv->location[index].limit_except.pop_back();
          }
          for (size_t i = 1; i < tokens.size(); i++) {
            std::transform(tokens[i].begin(),
                           tokens[i].end(),
                           tokens[i].begin(),
                           ::toupper);
            srv->location[index].limit_except.push_back(tokens[i]);
          }
        }
        if (tokens[0] == "client_max_body_size") {
          srv->location[index].client_max_body_size = _stoi(tokens[1]);
        }

        if (tokens[0] == "access_log") {
          srv->location[index].log["access_log"] = tokens[1];
        }

        if (tokens[0] == "error_log") {
          srv->location[index].log["error_log"] = tokens[1];
        }

        if (tokens[0] == "autoindex") {
          srv->location[index].autoindex = (tokens[1] == "on") ? true : false;
        }

        if (tokens[0] == "cgi") {
          srv->location[index].cgi["." + tokens[1]] = tokens[2];
        }

        // TODO(wleite): remove
        if (tokens[0] == "upload") {
          srv->location[index].upload = (tokens[1] == "on") ? true : false;
        }
        if (tokens[0] == "upload_store") {
          srv->location[index].upload_store = _trim(std::string(tokens[1]), "/");
        }
        if (line == "}") {
          break;
        }
      }
    }
  }
  srv->sockfd = -1;

  srv->print();

  return (srv);
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

void Config::_replace_unique(std::string* str, char pattern) {
  std::string tmp("");

  for (std::string::size_type i = 0; i < str->size() - 1; i++) {
    if (str->at(i) == pattern && str->at(i + 1) == pattern) {
      continue;
    }
    tmp += str->at(i);
  }
  *str = tmp;
}

std::string Config::_trim(const std::string& str, const std::string& set) {
  std::string tmp(str);

  tmp.erase(tmp.find_last_not_of(set) + 1);
  tmp.erase(0, tmp.find_first_not_of(set));

  return (tmp);
}

void Config::_trim_line(std::string* str, const std::string& set) {
  std::istringstream is(*str);
  std::string line;

  str->erase();
  while (std::getline(is, line)) {
    str->append(_trim(line, set));
    str->append("\n");
  }
}

std::vector<std::string> Config::_split(const std::string& str,
                                        const std::string& del) {
  std::vector<std::string> list;
  std::string s(str);
  size_t pos = 0;

  std::string token;
  while ((pos = s.find(del)) != std::string::npos) {
    token = s.substr(0, pos);
    list.push_back(token);
    s.erase(0, pos + del.length());
  }
  list.push_back(s);
  return (list);
}

size_t Config::_stoi(const std::string& str) {
  size_t n;
  std::istringstream(str) >> n;
  return (n);
}
