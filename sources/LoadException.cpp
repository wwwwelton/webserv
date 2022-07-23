
//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "LoadException.hpp"

std::string LoadException::_m = "";

LoadException::LoadException(void) {}

LoadException::LoadException(const std::string& str) { _m = str; }

LoadException::LoadException(const LoadException& src) { *this = src; }

LoadException::~LoadException(void) throw() {}

LoadException& LoadException::operator=(const LoadException& rhs) {
  _m = rhs._m;
  return (*this);
}

const char* LoadException::what(void) const throw() { return (_m.c_str()); }
