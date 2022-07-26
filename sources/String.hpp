//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef STRING_HPP_
#define STRING_HPP_

class String {
 public:
  String(void);
  String(const String& src);
  ~String(void);

  String& operator=(const String& rhs);
};

#endif  // STRING_HPP_
