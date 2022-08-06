//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef CONFIG_HELPER_HPP_
#define CONFIG_HELPER_HPP_

#define PARSE_ERROR "WebServ Failed to parse config: "

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "LoadException.hpp"
#include "String.hpp"
#include "defines.hpp"

class ConfigHelper {
 public:
  ConfigHelper(void);
  ConfigHelper(const ConfigHelper& src);
  ~ConfigHelper(void);

  ConfigHelper& operator=(const ConfigHelper& rhs);

  void set_tokens(const std::vector<std::string>& tokens);
  bool directive_already_exists(void);

  int get_backlog(void);
  std::pair<in_addr_t, int> get_listen(void);
  std::vector<std::string> get_server_name(void);
  std::string get_root(void);
  std::vector<std::string> get_index(void);
  std::string get_error_page(void);
  size_t get_timeout(void);
  int get_client_max_body_size(void);
  std::string get_access_log(void);
  std::string get_error_log(void);
  bool get_autoindex(void);
  std::string get_cgi(void);
  std::pair<int, std::string> get_redirect(void);
  std::vector<std::string> get_limit_except(void);
  bool get_upload(void);
  std::string get_upload_store(void);

 private:
  bool _valid_ip(const std::string& ip);
  bool _valid_port(const std::string& port);
  bool _valid_server_name(const std::string& server_name);
  bool _valid_index(const std::string& index);
  bool _valid_error_page(const std::string& error_page);
  bool _valid_log(const std::string& log);
  bool _valid_cgi_ext(const std::string& ext);
  bool _valid_cgi_bin(const std::string& bin);

  std::vector<std::string> _tokens;
  std::multiset<std::string> _list;

 public:
  class InvalidNumberArgs : public LoadException {
   public:
    explicit InvalidNumberArgs(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveDuplicate : public LoadException {
   public:
    explicit DirectiveDuplicate(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveInvValue : public LoadException {
   public:
    explicit DirectiveInvValue(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveUnknown : public LoadException {
   public:
    explicit DirectiveUnknown(const std::string& str);
    const char* what(void) const throw();
  };

  class InvFieldValue : public LoadException {
   public:
    explicit InvFieldValue(const std::string& field, const std::string& value);
    const char* what(void) const throw();
  };

  class NotSpecified : public LoadException {
   public:
    explicit NotSpecified(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveGlobal : public LoadException {
   public:
    explicit DirectiveGlobal(const std::string& str);
    const char* what(void) const throw();
  };

  class UnclosedBrackets : public LoadException {
   public:
    explicit UnclosedBrackets(const std::string& str);
    const char* what(void) const throw();
  };

  class SystemError : public LoadException {
   public:
    explicit SystemError(const std::string& field, const std::string& value);
    const char* what(void) const throw();
  };
};

#endif  // CONFIG_HELPER_HPP_
