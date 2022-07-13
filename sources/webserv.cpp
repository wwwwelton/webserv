// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#include "webserv.hpp"
#include "Logger.hpp"

WebServ::WebServ(int argc, char **argv) {
  size_t i;
  Config configs;
  (void)argc;

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);
  std::cout << clientlist.size() << "\n";

  configs = Config(argv);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    serverlist.insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds.push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }
}

int WebServ::_poll(void) {
  conn = poll((struct pollfd *)&(*pollfds.begin()), pollfds.size(), -1);
  // logger.debug() << "returned connections: " << conn << '\n';
  return conn;
}

void WebServ::_accept(int i) {
  Server *host = serverlist[pollfds[i].fd];
  int new_sd;

  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    clientlist[new_sd].request = new Request(new_sd);
    clientlist[new_sd].server = host;
    pollfds.push_back(_pollfd(new_sd, POLLIN));
    new_sd = accept(host->sockfd, NULL, NULL);
  }
}

void WebServ::_respond(int i) {
  int fd = pollfds[i].fd;
  RequestHandler req_handler;
  Request *ptr;
  Response res;

  //   req = Request(fd, clientlist[fd]);
  ptr = clientlist[fd].request->receive(fd);
  req_handler = RequestHandler(*ptr, clientlist[fd].server);
  req_handler.process();
  res = req_handler._response();
  res._send(fd);
  delete clientlist[fd].request;
  clientlist[fd].request = NULL;
  clientlist[fd].server = NULL;
  close(pollfds[i].fd);
  pollfds[i].fd = -1;
  compress = true;
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
