//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include <netdb.h>
#include <iostream>
#include "webserv.h"
#include "Server.hpp"
#include <set>

#define PORT 3490

int main(int argc, char** argv) {
  Server server1(PORT, 5);
  struct pollfd pollfd[200];
  int    connections;
  int    new_sd, nfds, currentsize;

  memset(pollfd, 0, sizeof(pollfd));
  pollfd[0].fd = server1.sockfd;
  pollfd[0].events = POLLIN;
  char buf[512];
  strncpy(buf,
          "HTTP/1.1 200 OK\n"
          "Content-Type: text/plain\n"
          "Content-Length: 12\n\n"
          "Hello world!\n", 74);
  nfds = 1;
  while (1) {
    connections = poll(pollfd, nfds, 60000);
    std::cout << "poll returned with " << connections << " connections\n";
    if (connections <= 0) {
      std::cout << "webserver timed out with no connections\n";
      break;
    }
    currentsize = nfds;
    for (int i = 0; i < nfds; i++) {
      if (pollfd[i].revents == 0) {
        std::cout << "no events in fd " << pollfd[i].fd << "\n";
        continue;
      }
      std::cout << "event found in fd " << pollfd[i].fd << "\n";
      if (pollfd[i].fd == server1.sockfd) {
        std::cout << "listening socket is readable\n";
        new_sd = accept(server1.sockfd, NULL, NULL);
        while (new_sd != -1) {
          pollfd[nfds].fd = new_sd;
          pollfd[nfds].events = POLLIN;
          std::cout << "connection received\n";
          nfds++;
          new_sd = accept(server1.sockfd, NULL, NULL);
        }
      }
      else {
        if (send(pollfd[i].fd, buf, 74, 0) < 0) {
          perror("send");
          close(pollfd[i].fd);
          exit(errno);
        }
        std::cout << "message sent\n";
        close(pollfd[i].fd);
        pollfd[i].fd = -1;
      }
    }
    std::cout << "end of loop, poll\n";
  }
  close(pollfd[0].fd);
  close(pollfd[1].fd);
  std::cout << "poll timed out. Connection closed\n";
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
