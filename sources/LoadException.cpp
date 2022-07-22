
//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "LoadException.hpp"

std::string LoadException::_message = "";

LoadException::LoadException(const std::string& str) {
  _message = str;
}

LoadException::~LoadException(void) throw() {
  return;
}

const char* LoadException::what(void) const throw() {
  return (_message.c_str());
}
