// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#include "webserv.h"

void accept_connections(std::vector<_pollfd> *pollfds,
                        std::map<int, Server *> *clientlist,
                        Server *host) {
  int new_sd;

  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    clientlist->insert(std::make_pair(new_sd, host));
    pollfds->push_back(_pollfd(new_sd, POLLIN));
    new_sd = accept(host->sockfd, NULL, NULL);
  }
}

void send_messages(int fd, std::map<int, Server *> *clientlist) {
  RequestHandler req_handler;
  Response res;

  //   req = Request(fd, clientlist[fd]);
  Request req = Request(fd);
  req_handler = RequestHandler(req, (*clientlist)[fd]);
  req_handler.process();
  res = req_handler._response();
  res._send(fd);
  clientlist->erase(fd);
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
  std::map<int, Server *> clientlist;
  std::vector<_pollfd> pollfds;
  int conn, compress = false;

  init(argc, argv, &serverlist, &pollfds);

  while (1) {
    conn = poll((struct pollfd *)&(*pollfds.begin()), pollfds.size(), 60000);
    std::cout << "returned connections: " << conn << '\n';
    if (conn <= 0)
      break;
    for (int i = 0, size = pollfds.size(); i < size; i++) {
      if (pollfds[i].revents == 0) {
        continue;
      }
      if (serverlist[pollfds[i].fd]) {
        std::cout << pollfds[i].fd << " socket has events\n";
        accept_connections(&pollfds, &clientlist, serverlist[pollfds[i].fd]);
      } else {
        std::cout << pollfds[i].fd << " client has events\n";
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
