//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

struct Comparator {
  bool operator()(const std::string& s1, const std::string& s2) const {
    std::string str1(s1.length(), ' ');
    std::string str2(s2.length(), ' ');
    std::transform(s1.begin(), s1.end(), str1.begin(), tolower);
    std::transform(s2.begin(), s2.end(), str2.begin(), tolower);
    return str1 < str2;
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
