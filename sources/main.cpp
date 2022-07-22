//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "WebServ.hpp"

void loop(int argc, char **argv) {
  WebServ webserv(argc, argv);

  while (true) {
    webserv._poll();
    if (webserv.conn <= 0)
      break;
    for (int i = 0, size = webserv.pollfds.size(); i < size; i++) {
      if (webserv.pollfds[i].revents == 0)
        continue;
      if (webserv.serverlist.count(webserv.pollfds[i].fd)) {
        webserv._accept(i);
      } else {
        webserv._respond(i);
      }
    }
    if (webserv.compress)
      webserv.purge_conns();
  }
}

int main(int argc, char **argv) {
  while (true) {
    try {
      loop(argc, argv);
      break;
    } catch (std::exception& e) {
      WebServ::log.error() << e.what() << "\n";
      WebServ::log.error() << "EXCEPTION HANDLED REINITIALIZING...\n";
    } catch (LoadException& e) {
      WebServ::log.error() << "Failed to read config file: " << e.what()
                           << std::endl;
      return (1);
    }
  }
}
