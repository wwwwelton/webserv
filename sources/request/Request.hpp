//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#include <functional>
#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

struct Comparator {
  static bool lower_less(char c1, char c2) {
    return std::tolower(c1) < std::tolower(c2);
  }

  bool operator()(const std::string& s1, const std::string& s2) const {
    return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), lower_less);
  }
};

struct Request {
  std::map<std::string, std::string, Comparator> headers;
  std::string method;
  std::string path;
  std::string http_version;
  std::string host;
  unsigned int error;
  int finished;

  Request();

  ~Request();

  bool is_valid() const;
};

std::ostream& operator<<(std::ostream& out, const Request& request);
#endif  // !HTTP_REQUEST_HPP
