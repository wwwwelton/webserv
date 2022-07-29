//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef STRING_HPP_
#define STRING_HPP_

#include <sstream>
#include <string>
#include <vector>

namespace String {

void replace_all(std::string* str, std::string old_word, std::string new_word);
void replace_unique(std::string* str, char pattern);
std::string trim(const std::string& str, const std::string& set);
std::string trim_last_if(const std::string& str, char c);
void trim_lines(std::string* str, const std::string& set);
std::vector<std::string> split(const std::string& str, const std::string& del);
int to_int(const std::string& str);

}  // namespace String

#endif  // STRING_HPP_
