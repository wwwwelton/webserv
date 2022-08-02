//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "String.hpp"

namespace String {

void replace_all(std::string* str, std::string old_word, std::string new_word) {
  size_t pos;

  pos = str->find(old_word);
  while (pos != std::string::npos) {
    str->replace(pos, old_word.length(), new_word);
    pos = str->find(old_word, pos + new_word.length());
  }
}

void replace_unique(std::string* str, char pattern) {
  std::string tmp("");

  for (std::string::size_type i = 0; i < str->size() - 1; i++) {
    if (str->at(i) == pattern && str->at(i + 1) == pattern) {
      continue;
    }
    tmp += str->at(i);
  }
  *str = tmp;
}

std::string trim(const std::string& str, const std::string& set) {
  std::string tmp(str);

  tmp.erase(tmp.find_last_not_of(set) + 1);
  tmp.erase(0, tmp.find_first_not_of(set));

  return (tmp);
}

std::string trim_last_if(const std::string& str, char c) {
  std::string tmp(str);

  if (str.size() > 1)
    if (str[str.size() - 1] == c)
      tmp = str.substr(0, str.size() - 1);

  return (tmp);
}

void trim_lines(std::string* str, const std::string& set) {
  std::istringstream is(*str);
  std::string line;

  str->erase();
  while (std::getline(is, line)) {
    str->append(trim(line, set));
    str->append("\n");
  }
}

std::vector<std::string> split(const std::string& str,
                               const std::string& del) {
  std::vector<std::string> list;
  std::string s(str);
  size_t pos = 0;

  std::string token;
  while ((pos = s.find(del)) != std::string::npos) {
    token = s.substr(0, pos);
    list.push_back(token);
    s.erase(0, pos + del.length());
  }
  list.push_back(s);
  return (list);
}

int to_int(const std::string& str) {
  size_t n;
  std::istringstream(str) >> n;
  return (n);
}

}  // namespace String
