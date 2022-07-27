//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ConfigHelper.hpp"

int ConfigHelper::get_backlog(const std::vector<std::string>& tokens) {
  static int setted = 0;

  if (setted)
    throw LoadException("Parse: workers already setted");
  if (tokens.size() != 2)
    throw LoadException("Parse: workers invalid");
  if (String::to_int(tokens[1]) <= 0 || String::to_int(tokens[1]) > 4096)
    throw LoadException("Parse: workers invalid");

  setted++;
  return (String::to_int(tokens[1]));
}
