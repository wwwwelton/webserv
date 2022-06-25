#include "webserv.h"
#include "Server.hpp"
#include <map>
#include <utility>
#include <iostream>

void init_servers(char **configfile, std::map<int, Server*> const& map) {
  int i = 0;
  while (configfile[i]) {
    Server *tmp = new Server;
    tmp->_socket();
    tmp->_bind(tmp->sockfd);
    tmp->_listen(500);

    map.insert(std::make_pair(tmp->sockfd, tmp));
  }
}

  int wait_events(pollfd *pollfds, int nfds) {
    int connections;

    connections = poll(pollfds, nfds, 60000);
    std::cout << "poll returned with " << connections << " connections\n";
    if (connections <= 0) {
      std::cout << "webserver timed out with no connections\n";
    }
    return connections;
  }

int main(int argc, char **argv) {
  std::map<int, Server*> serverlist;
  struct pollfd pollfds[200];
    serverlist.insert(std::make_pair(15, new Server));
  // config_file = readconfig(argv[1]);
  // init_servers(config_file);

  //struct  numero de servidores, portas
  // init servers -> colocar na lista -> colocar no pollfd


  // loop principal

  int nfds, connections, new_sd;
  char buf[5000];
  while (1) {
    connections = wait_events(pollfds, nfds);
    for (int i = 0; i < nfds; i++) {
      if (pollfds[i].revents == 0) {
        continue;
      }
      if (serverlist[pollfds[i].fd]) {
        new_sd = accept(pollfds[i].fd, NULL, NULL);
        while (new_sd != -1) {
          pollfds[nfds].fd = new_sd;
          pollfds[nfds].events = POLLIN;
          std::cout << "connection received\n";
          nfds++;
          new_sd = accept(pollfds[i].fd, NULL, NULL);
        }
      }
      else {
        if (send(pollfds[i].fd, buf, 74, 0) < 0) {
          perror("send");
          close(pollfds[i].fd);
          exit(errno);
        }
        std::cout << "message sent\n";
        close(pollfds[i].fd);
        pollfds[i].fd = -1;
      }
    }
  }
}
