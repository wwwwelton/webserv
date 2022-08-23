//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Config.hpp"

Config::Config(void) {
  backlog = DFL_BACKLOG;
}

Config::Config(const Config& src) {
  *this = src;
}

Config::~Config(void) {
  return;
}

Config& Config::operator=(const Config& rhs) {
  if (this != &rhs) {
    backlog = rhs.backlog;
    _servers = rhs._servers;
  }
  return (*this);
}

const Server& Config::operator[](size_t n) {
  return (_servers[n]);
}

size_t Config::size(void) {
  return (_servers.size());
}

void Config::load(char* file) {
  std::string tmp = _sanitize(_open(file));
  std::istringstream is(tmp);
  if (_is_valid(tmp))
    _parse(&is);
}

std::string Config::_open(char* file) {
  std::ifstream ifs;
  std::stringstream ss;

  ifs.open(file);
  ss << ifs.rdbuf();
  ifs.close();
  return (ss.str());
}

void Config::_purge_comments(std::string* str) {
  size_t pos1, pos2;

  pos1 = str->find("#");
  pos2 = str->find("\n", pos1);
  pos2 = (pos2 == std::string::npos) ? 1 : pos2;
  while (pos1 != std::string::npos) {
    str->erase(pos1, pos2 - pos1);
    pos1 = str->find("#");
    pos2 = str->find("\n", pos1);
  }
}

std::string Config::_sanitize(const std::string& file_content) {
  std::string tmp(file_content);

  _purge_comments(&tmp);

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

bool Config::_is_valid(const std::string& file_content) {
  int bl = 0;
  int br = 0;
  for (std::string::const_iterator it = file_content.begin();
       it != file_content.end();
       it++) {
    if (*it == '{')
      bl++;
    if (*it == '}')
      br++;
  }
  if (bl > br)
    throw ConfigHelper::UnclosedBrackets("{");
  if (br > bl)
    throw ConfigHelper::UnclosedBrackets("}");
  return (true);
}

std::vector<std::string> Config::_split_line(std::string line) {
  line = String::trim(line, " ");
  line = String::trim_last_if(line, ';');
  return (String::split(line, " "));
}

ServerLocation Config::_parse_location(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;
  ServerLocation location;
  ConfigHelper helper;

  while (std::getline(*is, line)) {
    tokens = _split_line(line);
    directive = tokens[0];

    helper.set_tokens(tokens);

    if (helper.directive_already_exists())
      throw ConfigHelper::DirectiveDuplicate(tokens[0]);
    if (directive == "root") {
      location.root = helper.get_root();
    } else if (directive == "index") {
      location.index = helper.get_index();
    } else if (directive == "limit_except") {
      location.limit_except = helper.get_limit_except();
    } else if (directive == "client_max_body_size") {
      location.client_max_body_size = helper.get_client_max_body_size();
    } else if (directive == "autoindex") {
      location.autoindex = helper.get_autoindex();
    } else if (directive == "cgi") {
      location.cgi[tokens[1]] = helper.get_cgi();
      cgi_list.insert(tokens[2]);
    } else if (directive == "return") {
      location.redirect = helper.get_redirect();
    } else if (directive == "upload") {
      location.upload = helper.get_upload();
    } else if (directive == "upload_store") {
      location.upload_store = helper.get_upload_store();
    } else if (directive[0] == '#') {
      continue;
    } else if (directive == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(directive);
    }
  }

  return (location);
}

Server Config::_parse_server(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;
  Server srv;
  ConfigHelper helper;

  while (std::getline(*is, line)) {
    tokens = _split_line(line);
    directive = tokens[0];

    helper.set_tokens(tokens);
    if (helper.directive_already_exists())
      throw ConfigHelper::DirectiveDuplicate(tokens[0]);
    if (directive == "listen") {
      srv.ip = helper.get_listen().first;
      srv.port = helper.get_listen().second;
    } else if (directive == "server_name") {
      srv.server_name = helper.get_server_name();
    } else if (directive == "root") {
      srv.root = helper.get_root();
    } else if (directive == "index") {
      srv.index = helper.get_index();
    } else if (directive == "error_page") {
      int code = String::to_int(tokens[1]);
      srv.error_page[code] = helper.get_error_page();
    } else if (directive == "timeout") {
      srv.timeout = helper.get_timeout();
    } else if (directive == "client_max_body_size") {
      srv.client_max_body_size = helper.get_client_max_body_size();
    } else if (directive == "access_log") {
      srv.log["access_log"] = helper.get_access_log();
    } else if (directive == "error_log") {
      srv.log["error_log"] = helper.get_error_log();
    } else if (directive == "autoindex") {
      srv.autoindex = helper.get_autoindex();
    } else if (directive == "cgi") {
      srv.cgi[tokens[1]] = helper.get_cgi();
      cgi_list.insert(tokens[2]);
    } else if (directive == "return") {
      srv.redirect = helper.get_redirect();
    } else if (directive == "upload") {
      srv.upload = helper.get_upload();
    } else if (directive == "upload_store") {
      srv.upload_store = helper.get_upload_store();
    } else if (directive == "location") {
      srv.location[tokens[1]] = _parse_location(is);
    } else if (directive[0] == '#') {
      continue;
    } else if (directive == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(directive);
    }
  }

  srv.fill();
  if (srv.is_invalid())
    throw ConfigHelper::NotSpecified(srv.error);

  return (srv);
}

void Config::_parse(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;
  ConfigHelper helper;

  while (std::getline(*is, line)) {
    tokens = _split_line(line);
    directive = tokens[0];

    helper.set_tokens(tokens);

    if (helper.directive_already_exists())
      throw ConfigHelper::DirectiveDuplicate(tokens[0]);
    if (directive == "workers" && _servers.size())
      throw ConfigHelper::DirectiveGlobal(tokens[0]);
    if (directive == "workers")
      backlog = helper.get_backlog();
    else if (directive == "server")
      _servers.push_back(_parse_server(is));
    else
      throw ConfigHelper::DirectiveUnknown(tokens[0]);
  }
}
