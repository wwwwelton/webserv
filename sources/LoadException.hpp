//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef LOAD_EXCEPTION_HPP
#define LOAD_EXCEPTION_HPP

#include <algorithm>
#include <string>

class LoadException : std::exception {
 protected:
  static std::string _message;

 public:
  explicit LoadException(const std::string& str);
  ~LoadException(void) throw();
  const char* what(void) const throw();
};

#endif  // LOAD_EXECPTION_HPP
