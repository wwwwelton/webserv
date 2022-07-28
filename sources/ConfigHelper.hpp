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

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "LoadException.hpp"
#include "String.hpp"
#include "defines.hpp"

class ConfigHelper {
 public:
  static int get_backlog(const std::vector<std::string>& tokens);

  static std::pair<in_addr_t, int>
  get_listen(const std::vector<std::string>& tokens);

  static std::vector<std::string>
  get_server_name(const std::vector<std::string>& tokens);

  static std::string get_root(const std::vector<std::string>& tokens);

  static std::vector<std::string>
  get_index(const std::vector<std::string>& tokens);

  static std::string get_error_page(const std::vector<std::string>& tokens);
  static int get_timeout(const std::vector<std::string>& tokens);
  static int get_client_max_body_size(const std::vector<std::string>& tokens);

 private:
  static bool _valid_ip(const std::string& ip);
  static bool _valid_port(const std::string& port);

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
};

#endif  // CONFIG_HELPER_HPP_
