//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef STRING_HPP_
#define STRING_HPP_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class String {
 public:
  String(void);
  String(const String& src);
  ~String(void);

  String& operator=(const String& rhs);

  static void replace_all(std::string* str,
                          const std::string& old_word,
                          const std::string& new_word);

  static void replace_unique(std::string* str, char pattern);

  static std::string trim(const std::string& str, const std::string& set);
  static void trim_lines(std::string* str, const std::string& set);

  static std::vector<std::string> split(const std::string& str,
                                        const std::string& del);

  static size_t stoi(const std::string& str);
};

#endif  // STRING_HPP_
