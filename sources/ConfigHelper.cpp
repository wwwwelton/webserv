//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ConfigHelper.hpp"

ConfigHelper::InvalidNumberArgs::InvalidNumberArgs(const std::string& str)
    : LoadException(str) {
  _m = PARSE_ERROR "invalid number of arguments in \"" + str + "\"";
}

const char* ConfigHelper::InvalidNumberArgs::what(void) const throw() {
  return (_m.c_str());
}

int ConfigHelper::get_backlog(const std::vector<std::string>& tokens) {
  static bool exists = false;

  if (exists)
    throw InvalidNumberArgs("workers already exists");
  if (tokens.size() != 2)
    throw InvalidNumberArgs("workers");
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 4096)
    throw InvalidNumberArgs("workers");

  exists = true;
  return (String::to_int(tokens[1]));
}
