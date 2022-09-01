//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef HTTPBASE_HPP
#define HTTPBASE_HPP

#define BUFFER_SIZE 100000
#define HEADER_SIZE 5000

class ResponseBase {
 public:
  static char buffer_req[BUFFER_SIZE];
  static char buffer_resp[HEADER_SIZE + BUFFER_SIZE];
  static int size;
};

#endif  // HTTPBASE_HPP
