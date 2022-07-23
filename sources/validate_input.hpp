//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstring>
#include <fstream>
#include <string>

#include "LoadException.hpp"
#include "defines.hpp"

class ValidateInputException : public LoadException {
 public:
  explicit ValidateInputException(const std::string& str);
  const char* what(void) const throw();
};

void validate_input(int argc, char** argv);

#endif  // UTILS_HPP
