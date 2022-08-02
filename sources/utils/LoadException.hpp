//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef LOAD_EXCEPTION_HPP
#define LOAD_EXCEPTION_HPP

#include <string>

class LoadException : std::exception {
 protected:
  static std::string _m;

 public:
  LoadException(void);
  explicit LoadException(const std::string& str);
  LoadException(const LoadException& src);
  virtual ~LoadException(void) throw();

  LoadException& operator=(const LoadException& rhs);

  const char* what(void) const throw();
};

#endif  // LOAD_EXECPTION_HPP
