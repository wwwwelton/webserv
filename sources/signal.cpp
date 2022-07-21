//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "WebServ.hpp"

void sighandler(const int signal, void *ptr) {
  static WebServ *webserv = NULL;

  if (webserv == NULL)
    webserv = reinterpret_cast<WebServ *>(ptr);
  if (signal == SIGINT || signal == SIGQUIT) {
    webserv->~WebServ();
    exit(128 + signal);
  }
}
