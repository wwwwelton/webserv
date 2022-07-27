//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ConfigHelper.hpp"

int ConfigHelper::get_backlog(const std::vector<std::string>& tokens) {
  static bool exists = false;

  if (exists)
    throw DirectiveDuplicate(tokens[0]);
  if (tokens.size() != 2)
    throw InvalidNumberArgs(tokens[0]);
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 4096)
    throw DirectiveInvValue(tokens[0]);

  exists = true;
  return (String::to_int(tokens[1]));
}

in_addr_t ConfigHelper::get_ip(const std::vector<std::string>& tokens) {
  in_addr_t ip;

  if (String::split(tokens[1], " ").size() != 1) {
    throw InvalidNumberArgs(tokens[0]);
  }

  if (tokens[1].find(":") != std::string::npos) {
    if (*tokens[1].begin() == ':')
      throw InvFieldValue("host", tokens[1]);
  }

  if (tokens[1].find(":") != std::string::npos) {
    std::vector<std::string> tmp = String::split(tokens[1], ":");
    if (tmp[0].find_first_not_of("0123456789.") != std::string::npos)
      throw InvFieldValue("host", tokens[1]);
    if (tmp[0].find(".") == std::string::npos)
      throw InvFieldValue("host", tokens[1]);
    ip = inet_addr(tmp[0].c_str());
  } else if (tokens[1].find(".") != std::string::npos) {
    if (tokens[1].find_first_not_of("0123456789.") != std::string::npos)
      throw InvFieldValue("host", tokens[1]);
    ip = inet_addr(tokens[1].c_str());
  } else {
    ip = inet_addr(DFL_ADDRESS);
  }
  if (ip == 0)
    throw InvFieldValue("host", tokens[1]);
  return (ip);
}

int ConfigHelper::get_port(const std::vector<std::string>& tokens) {
  int port;

  if (String::split(tokens[1], " ").size() != 1) {
    throw InvalidNumberArgs(tokens[0]);
  }

  if (tokens[1].find(":") != std::string::npos) {
    if (*tokens[1].rbegin() == ':')
      throw InvFieldValue("port", tokens[1]);
  }

  if (tokens[1].find(":") != std::string::npos) {
    std::vector<std::string> tmp = String::split(tokens[1], ":");
    if (tmp[1].find_first_not_of("0123456789") != std::string::npos)
      throw InvFieldValue("port", tokens[1]);
    port = htons(String::to_int(tmp[1]));
  } else if (tokens[1].find(".") == std::string::npos) {
    if (tokens[1].find_first_not_of("0123456789") != std::string::npos)
      throw InvFieldValue("port", tokens[1]);
    port = htons(String::to_int(tokens[1]));
  } else {
    port = htons(DFL_PORT);
  }
  if (port == 0)
    throw InvFieldValue("port", tokens[1]);
  return (port);
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
