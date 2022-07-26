//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "String.hpp"

String::String(void) {}

String::String(const String& src) { *this = src; }

String::~String(void) {}

String& String::operator=(const String& rhs) {
  (void)rhs;
  return (*this);
}
