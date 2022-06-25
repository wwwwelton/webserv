//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include "webserv.h"

int accept_connections(pollfd *pollfds, int socketfd, int nfds) {
  int new_sd;

  new_sd = accept(socketfd, NULL, NULL);
  while (new_sd != -1) {
    pollfds[nfds].fd = new_sd;
    pollfds[nfds].events = POLLIN;
    nfds++;
    new_sd = accept(socketfd, NULL, NULL);
  }
  return nfds;
}

void send_messages(pollfd *pollfds, int i, char *buf, char *buf2) {
  int rc;
  (void)rc;
  rc = recv(pollfds[i].fd, buf, sizeof(buf) - 74, 0);
  rc = send(pollfds[i].fd, buf2, 74, 0);
  close(pollfds[i].fd);
  pollfds[i].fd = -1;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  struct pollfd pollfd[200];
  memset(pollfd, 0, sizeof(pollfd));
  std::map<int, Server*> serverlist;
  std::map<int, int>     clientlist;


  char buf[512000];
  char buf2[512000];
  strncpy(buf2,
          "HTTP/1.1 200 OK\n"
          "Content-Type: text/plain\n"
          "Content-Length: 12\n\n"
          "Hello world!\n", 74);


  int connections;
  int nfds, currentsize;
  nfds = init(argv, &serverlist, pollfd);
  (void)currentsize;
  while (1) {
    connections = poll(pollfd, nfds, 60000);
    if (connections <= 0) {
      break;
    }
    currentsize = nfds;
    for (int i = 0; i < nfds; i++) {
      if (pollfd[i].revents == 0)
        continue;
      if (serverlist[pollfd[i].fd])
        nfds = accept_connections(pollfd, pollfd[i].fd, nfds);
      else
        send_messages(pollfd, i, buf, buf2);
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
