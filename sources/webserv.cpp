//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "webserv.hpp"
#include "Logger.hpp"

Logger WebServ::log = WebServ::init_log();
Logger WebServ::init_log(void) {
  Logger logger(LOG_LEVEL);
  return logger;
}

WebServ::WebServ(int argc, char **argv) {
  log.info() << "Initializing WebServ\n";
  size_t i;
  Config configs;
  (void)argc;

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);

  configs = Config(argv);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    serverlist.insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds.push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }
}

int WebServ::_poll(void) {
  conn = poll((struct pollfd *)&(*pollfds.begin()), pollfds.size(), -1);
  log.info() << "returned connections: " << conn << '\n';
  return conn;
}

void WebServ::_accept(int i) {
  Server *host = serverlist[pollfds[i].fd];
  int new_sd;

  log.info() << "Events detected in socket " << pollfds[i].fd << "\n";
  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    clientlist[new_sd].request = new Request(new_sd);
    clientlist[new_sd].server = host;
    pollfds.push_back(_pollfd(new_sd, POLLIN));
    log.info() << host->server_name[0]
               << " accepted connection of client "
               << new_sd << "\n";
    new_sd = accept(host->sockfd, NULL, NULL);
  }
}

void WebServ::_respond(int i) {
  int fd = pollfds[i].fd;
  Request *ptr;

  ptr = clientlist[fd].request->receive(fd);
  if (ptr->finished) {
    Response req_handler(*ptr, clientlist[fd].server);
    req_handler.process();
    req_handler._send(fd);
    delete clientlist[fd].request;
    clientlist[fd].request = NULL;
    clientlist[fd].server = NULL;
    close(pollfds[i].fd);
    pollfds[i].fd = -1;
    compress = true;
  }
}

void WebServ::purge_conns(void) {
  std::vector<_pollfd>::iterator it = pollfds.begin();
  std::vector<_pollfd>::iterator ite = pollfds.end();
  for (; it != ite; it++) {
    if (it->fd == -1) {
      pollfds.erase(it);
      ite--;
      if (it == ite)
        break;
    }
  }
}
