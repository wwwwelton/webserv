//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPBASE_HPP
#define HTTPBASE_HPP

#define BUFFER_SIZE 2000
#define HEADER_SIZE 2000

class HttpBase {
 public:
  static char buffer_req[BUFFER_SIZE];
  static char buffer_resp[HEADER_SIZE + BUFFER_SIZE];
  static int size;
};

#endif  // HTTPBASE_HPP
