//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "String.hpp"

String::String(void) {}

String::String(const String& src) { *this = src; }

String::~String(void) {}

String& String::operator=(const String& rhs) {
  (void)rhs;
  return (*this);
}

void String::replace_all(std::string* str,
                         const std::string& old_word,
                         const std::string& new_word) {
  size_t pos;

  pos = str->find(old_word);
  while (pos != std::string::npos) {
    str->replace(pos, old_word.length(), new_word);
    pos = str->find(old_word, pos + new_word.length());
  }
}

void String::replace_unique(std::string* str, char pattern) {
  std::string tmp("");

  for (std::string::size_type i = 0; i < str->size() - 1; i++) {
    if (str->at(i) == pattern && str->at(i + 1) == pattern) {
      continue;
    }
    tmp += str->at(i);
  }
  *str = tmp;
}

std::string String::trim(const std::string& str, const std::string& set) {
  std::string tmp(str);

  tmp.erase(tmp.find_last_not_of(set) + 1);
  tmp.erase(0, tmp.find_first_not_of(set));

  return (tmp);
}

void String::trim_lines(std::string* str, const std::string& set) {
  std::istringstream is(*str);
  std::string line;

  str->erase();
  while (std::getline(is, line)) {
    str->append(trim(line, set));
    str->append("\n");
  }
}

std::vector<std::string> String::split(const std::string& str,
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

size_t String::stoi(const std::string& str) {
  size_t n;
  std::istringstream(str) >> n;
  return (n);
}
