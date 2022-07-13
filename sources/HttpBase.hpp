//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

#ifndef HTTPBASE_HPP
#define HTTPBASE_HPP

class HttpBase {
 public:
  static char buffer_req[512000];
  static char buffer_resp[512000];
  static int size;
};

#endif  // HTTPBASE_HPP
