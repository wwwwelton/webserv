//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ConfigHelper.hpp"

int ConfigHelper::get_backlog(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 4096)
    throw DirectiveInvValue(tokens[0]);
  return (String::to_int(tokens[1]));
}

std::pair<in_addr_t, int>
ConfigHelper::get_listen(const std::vector<std::string>& tokens) {
  in_addr_t ip;
  int port;

  if (String::split(tokens[1], " ").size() != 1) {
    throw InvalidNumberArgs(tokens[0]);
  }
  if (tokens[1].find(":") != std::string::npos) {
    std::vector<std::string> tmp = String::split(tokens[1], ":");
    if (tmp.size() != 2)
      throw InvFieldValue("host/port", tokens[1]);
    if (_valid_ip(tmp[0]) && _valid_port(tmp[1])) {
      ip = inet_addr(tmp[0].c_str());
      port = htons(String::to_int(tmp[1]));
    } else if (!_valid_ip(tmp[0])) {
      throw InvFieldValue("host", tokens[1]);
    } else {
      throw InvFieldValue("port", tokens[1]);
    }
    return (std::make_pair(ip, port));
  }
  if (_valid_ip(tokens[1]) && !_valid_port(tokens[1])) {
    ip = inet_addr(tokens[1].c_str());
    port = htons(DFL_PORT);
  } else if (!_valid_ip(tokens[1]) && _valid_port(tokens[1])) {
    ip = inet_addr(DFL_ADDRESS);
    port = htons(String::to_int(tokens[1]));
  } else {
    throw InvFieldValue("host/port", tokens[1]);
  }
  return (std::make_pair(ip, port));
}

std::vector<std::string>
ConfigHelper::get_server_name(const std::vector<std::string>& tokens) {
  if (tokens.size() == 1)
    throw InvalidNumberArgs(tokens[0]);
  return (String::split(tokens[1], " "));
}

std::string ConfigHelper::get_root(const std::vector<std::string>& tokens) {
  if (tokens.size() == 1)
    throw InvalidNumberArgs(tokens[0]);
  return (String::trim(std::string(tokens[1]), "/"));
}

std::vector<std::string>
ConfigHelper::get_index(const std::vector<std::string>& tokens) {
  if (tokens.size() == 1)
    throw InvalidNumberArgs(tokens[0]);
  return (String::split(tokens[1], " "));
}

std::string
ConfigHelper::get_error_page(const std::vector<std::string>& tokens) {
  if (tokens.size() != 3)
    throw InvalidNumberArgs(tokens[0]);
  if (tokens[1].find_first_not_of("0123456789") != std::string::npos ||
      String::to_int(tokens[1]) > 499 || String::to_int(tokens[1]) < 400) {
    throw InvFieldValue("error_page", tokens[1]);
  }
  return (String::trim(std::string(tokens[2]), "/"));
}

int ConfigHelper::get_timeout(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 4096)
    throw DirectiveInvValue(tokens[0]);
  return (String::to_int(tokens[1]));
}

int ConfigHelper::get_client_max_body_size(const std::vector<std::string>&
                                               tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 1000)
    throw DirectiveInvValue(tokens[0]);
  return (String::to_int(tokens[1]));
}

std::string
ConfigHelper::get_access_log(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  return (tokens[1]);
}

std::string
ConfigHelper::get_error_log(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  return (tokens[1]);
}

#include <iostream>
bool ConfigHelper::get_autoindex(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  if (tokens[1] != "on" && tokens[1] != "off")
    throw InvFieldValue("autoindex", tokens[1]);
  return ((tokens[1] == "on") ? true : false);
}

std::string ConfigHelper::get_cgi(const std::vector<std::string>& tokens) {
  if (tokens.size() != 3)
    throw InvalidNumberArgs(tokens[0]);
  return (tokens[2]);
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
  _m = PARSE_ERROR "invalid " + field + " in " + "\"" + value + "\"";
}

const char* ConfigHelper::InvFieldValue::what(void) const throw() {
  return (_m.c_str());
}
