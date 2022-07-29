//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ConfigHelper.hpp"

ConfigHelper::ConfigHelper(void) {
  return;
}

ConfigHelper::ConfigHelper(const ConfigHelper& src) {
  *this = src;
}

ConfigHelper::~ConfigHelper(void) {
  return;
}

ConfigHelper& ConfigHelper::operator=(const ConfigHelper& rhs) {
  if (this != &rhs) {
    _tokens = rhs._tokens;
  }
  return (*this);
}

void ConfigHelper::set_tokens(const std::vector<std::string>& tokens) {
  std::string tmp;

  _tokens = tokens;

  tmp = CFG_FIELD_UNIQUE;
  if (tmp.find(tokens[0]) != std::string::npos ||
      tokens.size() <= 1 || tokens[0][0] == '#')
    return;
  tmp = CFG_FIELD_DOUBLE;
  if (tmp.find(tokens[0]) != std::string::npos)
    _list.insert(tokens[0] + tokens[1]);
  else
    _list.insert(tokens[0]);
}

bool ConfigHelper::directive_already_exists(void) {
  std::string tmp;
  std::string elem;

  tmp = CFG_FIELD_DOUBLE;
  if (tmp.find(_tokens[0]) != std::string::npos)
    elem = _tokens[0] + _tokens[1];
  else
    elem = _tokens[0];
  if (_list.count(elem) > 1)
    return (true);
  return (false);
}

int ConfigHelper::get_backlog(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  int backlog = String::to_int(_tokens[1]);
  if (backlog <= CFG_MIN_BACKLOG || backlog > CFG_MAX_BACKLOG)
    throw DirectiveInvValue(_tokens[0]);
  return (backlog);
}

std::pair<in_addr_t, int> ConfigHelper::get_listen(void) {
  in_addr_t ip;
  int port;

  if (String::split(_tokens[1], " ").size() != 1)
    throw InvalidNumberArgs(_tokens[0]);
  if (_tokens[1].find(":") != std::string::npos) {
    std::vector<std::string> tmp = String::split(_tokens[1], ":");
    if (tmp.size() != 2)
      throw InvFieldValue("host/port", _tokens[1]);
    if (_valid_ip(tmp[0]) && _valid_port(tmp[1])) {
      ip = inet_addr(tmp[0].c_str());
      port = htons(String::to_int(tmp[1]));
    } else if (!_valid_ip(tmp[0])) {
      throw InvFieldValue("host", _tokens[1]);
    } else {
      throw InvFieldValue("port", _tokens[1]);
    }
    return (std::make_pair(ip, port));
  }
  if (_valid_ip(_tokens[1]) && !_valid_port(_tokens[1])) {
    ip = inet_addr(_tokens[1].c_str());
    port = htons(DFL_PORT);
  } else if (!_valid_ip(_tokens[1]) && _valid_port(_tokens[1])) {
    ip = inet_addr(DFL_ADDRESS);
    port = htons(String::to_int(_tokens[1]));
  } else {
    throw InvFieldValue("host/port", _tokens[1]);
  }
  return (std::make_pair(ip, port));
}

std::vector<std::string> ConfigHelper::get_server_name(void) {
  if (_tokens.size() == 1)
    throw InvalidNumberArgs(_tokens[0]);
  for (size_t i = 1; i < _tokens.size(); i++) {
    if (!_valid_server_name(_tokens[i]))
      throw InvFieldValue("server_name", _tokens[i]);
  }
  std::vector<std::string> tmp(_tokens.begin() + 1, _tokens.end());
  for (size_t i = 0; i < tmp.size(); i++) {
    std::transform(tmp[i].begin(), tmp[i].end(), tmp[i].begin(), ::tolower);
  }
  return (tmp);
}

std::string ConfigHelper::get_root(void) {
  if (_tokens.size() == 1)
    throw InvalidNumberArgs(_tokens[0]);
  struct stat buffer;
  if (stat(_tokens[1].c_str(), &buffer) != 0)
    throw InvFieldValue("root", _tokens[1]);
  return (String::trim(std::string(_tokens[1]), "/"));
}

std::vector<std::string> ConfigHelper::get_index(void) {
  if (_tokens.size() == 1)
    throw InvalidNumberArgs(_tokens[0]);
  for (size_t i = 1; i < _tokens.size(); i++) {
    if (!_valid_index(_tokens[i]))
      throw InvFieldValue("index", _tokens[i]);
  }
  std::vector<std::string> tmp(_tokens.begin() + 1, _tokens.end());
  return (tmp);
}

std::string ConfigHelper::get_error_page(void) {
  if (_tokens.size() != 3)
    throw InvalidNumberArgs(_tokens[0]);
  if (_tokens[1].find_first_not_of("0123456789") != std::string::npos ||
      String::to_int(_tokens[1]) > CFG_MAX_ERR_CODE ||
      String::to_int(_tokens[1]) < CFG_MIN_ERR_CODE) {
    throw InvFieldValue("error_page", _tokens[1]);
  }
  if (!_valid_error_page(_tokens[2]))
    throw InvFieldValue("error_page", _tokens[2]);
  return (String::trim(std::string(_tokens[2]), "/"));
}

int ConfigHelper::get_timeout(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  if (String::to_int(_tokens[1]) <= CFG_MIN_TIMEOUT ||
      String::to_int(_tokens[1]) > CFG_MAX_TIMEOUT)
    throw DirectiveInvValue(_tokens[0]);
  return (String::to_int(_tokens[1]));
}

int ConfigHelper::get_client_max_body_size(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  if (String::to_int(_tokens[1]) <= 0 || String::to_int(_tokens[1]) > 1000)
    throw DirectiveInvValue(_tokens[0]);
  return (String::to_int(_tokens[1]));
}

std::string ConfigHelper::get_access_log(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  return (_tokens[1]);
}

std::string ConfigHelper::get_error_log(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  return (_tokens[1]);
}

bool ConfigHelper::get_autoindex(void) {
  if (_tokens.size() != 2)
    throw InvalidNumberArgs(_tokens[0]);
  if (_tokens[1] != "on" && _tokens[1] != "off")
    throw InvFieldValue("autoindex", _tokens[1]);
  return ((_tokens[1] == "on") ? true : false);
}

std::string ConfigHelper::get_cgi(void) {
  if (_tokens.size() != 3)
    throw InvalidNumberArgs(_tokens[0]);
  return (_tokens[2]);
}

std::pair<int, std::string> ConfigHelper::get_redirect(void) {
  if (_tokens.size() != 3)
    throw InvalidNumberArgs(_tokens[0]);
  if (_tokens[1].find_first_not_of("0123456789") != std::string::npos ||
      String::to_int(_tokens[1]) > 499 || String::to_int(_tokens[1]) < 100) {
    throw InvFieldValue("redirect", _tokens[1]);
  }
  return (std::make_pair(String::to_int(_tokens[1]), _tokens[2]));
}

std::vector<std::string> ConfigHelper::get_limit_except(void) {
  if (_tokens.size() == 1)
    throw InvalidNumberArgs(_tokens[0]);
  std::vector<std::string> tmp(_tokens.begin() + 1, _tokens.end());
  for (size_t i = 0; i < tmp.size(); i++) {
    std::transform(tmp[i].begin(), tmp[i].end(), tmp[i].begin(), ::toupper);
  }
  return (tmp);
}

bool ConfigHelper::_valid_ip(const std::string& ip) {
  std::vector<std::string> list = String::split(ip, ".");

  if (list.size() != 4)
    return (false);
  for (size_t i = 0; i < list.size(); i++) {
    if (list[i].find_first_not_of("0123456789") != std::string::npos ||
        String::to_int(list[i]) > 255 || String::to_int(list[i]) < 0) {
      return (false);
    }
  }
  return (true);
}

bool ConfigHelper::_valid_port(const std::string& port) {
  if (port.find_first_not_of("0123456789") != std::string::npos ||
      String::to_int(port) > 65000 || String::to_int(port) < 80) {
    return (false);
  }
  return (true);
}

bool ConfigHelper::_valid_server_name(const std::string& server_name) {
  char start = server_name[0];
  char end = server_name[server_name.size() - 1];
  if (!::isalnum(start) || !::isalnum(end))
    return (false);
  for (std::string::const_iterator it = server_name.begin();
       it != server_name.end();
       it++) {
    if (!::isalnum(*it) && *it != '.' && *it != '-' && *it != '_')
      return (false);
  }
  return (true);
}

bool ConfigHelper::_valid_index(const std::string& index) {
  char start = index[0];
  char end = index[index.size() - 1];
  if (!::isalnum(start) || !::isalnum(end))
    return (false);
  for (std::string::const_iterator it = index.begin();
       it != index.end();
       it++) {
    if (!::isalnum(*it) && *it != '.' && *it != '-' && *it != '_')
      return (false);
  }
  if (index.find(".") == std::string::npos)
    return (false);
  return (true);
}

bool ConfigHelper::_valid_error_page(const std::string& error_page) {
  char start = error_page[0];
  char end = error_page[error_page.size() - 1];
  if ((!::isalnum(start) && start != '/') || !::isalnum(end))
    return (false);
  for (std::string::const_iterator it = error_page.begin();
       it != error_page.end();
       it++) {
    if (!::isalnum(*it) && *it != '.' && *it != '-' && *it != '_' && *it != '/')
      return (false);
  }
  if (error_page.find(".") == std::string::npos)
    return (false);
  return (true);
}

ConfigHelper::InvalidNumberArgs::InvalidNumberArgs(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "invalid number of arguments in \"" + str + "\"";
}

const char* ConfigHelper::InvalidNumberArgs::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::DirectiveDuplicate::DirectiveDuplicate(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "directive is duplicate \"" + str + "\"";
}

const char* ConfigHelper::DirectiveDuplicate::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::DirectiveInvValue ::DirectiveInvValue(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "directive invalid value in \"" + str + "\"";
}

const char* ConfigHelper::DirectiveInvValue::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::DirectiveUnknown ::DirectiveUnknown(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "unknown directive \"" + str + "\"";
}

const char* ConfigHelper::DirectiveUnknown::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::InvFieldValue::InvFieldValue(const std::string& field,
                                           const std::string& value)
    : LoadException("") {
  _m = PARSE_ERROR "invalid \"" + value + "\" in " + field;
}

const char* ConfigHelper::InvFieldValue::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::NotSpecified ::NotSpecified(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "\"" + str + "\" not specified";
}

const char* ConfigHelper::NotSpecified::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::DirectiveGlobal ::DirectiveGlobal(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "\"" + str + "\" must be specified before server blocks";
}

const char* ConfigHelper::DirectiveGlobal::what(void) const throw() {
  return (_m.c_str());
}

ConfigHelper::UnclosedBrackets ::UnclosedBrackets(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "unclosed brackets \"" + str + "\"";
}

const char* ConfigHelper::UnclosedBrackets::what(void) const throw() {
  return (_m.c_str());
}
