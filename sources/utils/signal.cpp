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
