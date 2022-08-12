//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "signal.hpp"

void sighandler(const int signal, void *ptr) {
  static WebServ *webserv = NULL;
  if (webserv == NULL)
    webserv = reinterpret_cast<WebServ *>(ptr);
  if (signal == SIGINT || signal == SIGQUIT) {
    webserv->~WebServ();
    exit(128 + signal);
  }
}

void init_signals(WebServ *ptr) {
  void *func = NULL;
  func = reinterpret_cast<void *>(sighandler);
  std::signal(SIGINT, reinterpret_cast<__sighandler_t>(func));
  std::signal(SIGQUIT, reinterpret_cast<__sighandler_t>(func));
  sighandler(0, ptr);
}

void kill_ghost_in_the_shell(const Config &conf) {
  for (std::set<std::string>::iterator
           it = conf.cgi_list.begin();
       it != conf.cgi_list.end(); it++) {
    std::vector<std::string> vec = String::split(*it, "/");
    std::string cgi = *(vec.end() - 1);
    cgi = "pkill -9 -f " + cgi;
    std::system(cgi.c_str());
  }
}
