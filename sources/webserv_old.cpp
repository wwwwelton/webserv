//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Logger.hpp"

int init(int argc,
         char** argv,
         std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds) {
  (void)argc;

  size_t i;
  Config configs;

  configs = Config(argv);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    map->insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds->push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }

  return i;
}


void accept_connections(std::vector<_pollfd> *pollfds,
                        std::vector<req> *clientlist,
                        Server *host) {
  int new_sd;

  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    (*clientlist)[new_sd].request = new Request(new_sd);
    (*clientlist)[new_sd].server = host;
    pollfds->push_back(_pollfd(new_sd, POLLIN));
    new_sd = accept(host->sockfd, NULL, NULL);
  }
}

void send_messages(int fd, std::vector<req> *clientlist) {
  RequestHandler req_handler;
  Request *ptr;
  Response res;

  //   req = Request(fd, clientlist[fd]);
  ptr = (*clientlist)[fd].request->receive();
  req_handler = RequestHandler(*ptr, (*clientlist)[fd].server);
  req_handler.process();
  res = req_handler._response();
  res._send(fd);
  delete (*clientlist)[fd].request;
  (*clientlist)[fd].request = NULL;
  (*clientlist)[fd].server = NULL;
}

void compress_array(std::vector<_pollfd> *pollfds) {
  std::vector<_pollfd>::iterator it = pollfds->begin();
  std::vector<_pollfd>::iterator ite = pollfds->end();
  for (; it != ite; it++) {
    if (it->fd == -1)
      pollfds->erase(it);
  }
}

int main(int argc, char **argv) {
  std::map<int, Server *> serverlist;
  std::vector<req>        clientlist;
  std::vector<_pollfd>    pollfds;
  int conn, compress = false;
  Logger logger(LVL_DEBUG);

  clientlist.reserve(1024);
  init(argc, argv, &serverlist, &pollfds);
  logger.debug() << "initializing webserv" << std::endl;

  while (1) {
    conn = poll((struct pollfd *)&(*pollfds.begin()), pollfds.size(), -1);
    logger.debug() << "returned connections: " << conn << '\n';
    if (conn <= 0)
      break;
    for (int i = 0, size = pollfds.size(); i < size; i++) {
      if (pollfds[i].revents == 0) {
        continue;
      }
      logger.info() << "events detected on socket " << pollfds[i].fd << std::endl;
      if (serverlist.count(pollfds[i].fd)) {
        accept_connections(&pollfds, &clientlist, serverlist[pollfds[i].fd]);
      } else {
        send_messages(pollfds[i].fd, &clientlist);
        close(pollfds[i].fd);
        pollfds[i].fd = -1;
        compress = true;
      }
    }
    if (compress) {
      compress = false;
      compress_array(&pollfds);
    }
  }
  for (size_t i = 0; i < pollfds.size(); i++)
    close(pollfds[i].fd);
}
