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
  Request req;
  RequestHandler req_handler;
  Response res;

  //   req = Request(fd, clientlist[fd]);
  req = Request(fd);
  req_handler = RequestHandler(req);
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
  (void)argc;
  (void)argv;
  std::map<int, Server *> serverlist;
  std::map<int, Server *> clientlist;
  std::vector<_pollfd> pollfds;
  int conn, compress = false;

  init(argv, &serverlist, &pollfds);
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

// s_addrinfo *result, *rp;
// s_addrinfo hints;
// memset(&hints, 0, sizeof(s_addrinfo));
// hints.ai_family = AF_INET;      /* Allow IPv4 */
// hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
// hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */ // AI_PASSIVE
// hints.ai_protocol = 0;          /* Any protocol */
// hints.ai_canonname = NULL;
// hints.ai_addr = NULL;
// hints.ai_next = NULL;

// if (getaddrinfo(NULL, PORT, &hints, &result)) {
//   std::cerr << "error: addrinfo\n";
//   exit(EXIT_FAILURE);
// }
