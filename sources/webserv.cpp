//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include <set>
#include <map>
#include <netdb.h>
#include <iostream>
#include <utility>
#include "webserv.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  Server server1(PORT1, 5);
  struct pollfd pollfd[200];
  memset(pollfd, 0, sizeof(pollfd));
  std::map<int, Server*> serverlist;
        // socketFD, server
  std::map<int, int>     clientlist;
        // clientFD, socketFD
  pollfd[0].fd = server1.sockfd;
  pollfd[0].events = POLLIN;


  char buf[512000];
  char buf2[512000];
  strncpy(buf2,
          "HTTP/1.1 200 OK\n"
          "Content-Type: text/plain\n"
          "Content-Length: 12\n\n"
          "Hello world!\n", 74);
  serverlist.insert(std::make_pair(server1.sockfd, &server1));


  int rc;
  int    connections;
  int    new_sd, nfds, currentsize;
  nfds = 1;
  (void)rc;
  (void)currentsize;
  while (1) {
    connections = poll(pollfd, nfds, 60000);
    if (connections <= 0) {
      break;
    }
    currentsize = nfds;
    for (int i = 0; i < nfds; i++) {
      if (pollfd[i].revents == 0) {
        continue;
      }
      if (serverlist[pollfd[i].fd]) {
        new_sd = accept(server1.sockfd, NULL, NULL);
        while (new_sd != -1) {
          pollfd[nfds].fd = new_sd;
          pollfd[nfds].events = POLLIN;
          nfds++;
          new_sd = accept(server1.sockfd, NULL, NULL);
        }
      }
      else {
        rc = recv(pollfd[i].fd, buf, sizeof(buf) - 74, 0);
        rc = send(pollfd[i].fd, buf2, 74, 0);
        close(pollfd[i].fd);
        pollfd[i].fd = -1;
      }
    }
  }
  // mem move poll fds
  close(pollfd[0].fd);
  close(pollfd[1].fd);
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
