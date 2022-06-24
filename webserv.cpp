//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include <netdb.h>
#include <iostream>
#include "webserv.h"
#include <set>

#define PORT 3490

int main(int argc, char **argv) {
  struct sockaddr_in sockaddress;


  int connections;
  int  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //server( socket(AF_INET, SOCK_STREAM, 0), port);
  // insert(15, server);

  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, )
  if (sockfd == -1) {
    perror("socket");
    exit(errno);
  }
  sockaddress.sin_family = AF_INET;
  sockaddress.sin_port = htons(PORT);
  sockaddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  std::cout << sizeof(sockaddr) << " " << sizeof(sockaddr_in) << "\n";
  if (bind(sockfd, (const sockaddr *)&sockaddress, sizeof(sockaddr_in))) {
    perror("bind");
    close(sockfd);
    exit(errno);
  }
  if (listen(sockfd, 5)) {
    perror("listen");
    exit(errno);
  }
  struct pollfd pollfd[200];
  memset(pollfd, 0, sizeof(pollfd));
  pollfd[0].fd = sockfd;
  pollfd[0].events = POLLIN;
  char buf[512];
  strncpy(buf, \
  "HTTP/1.1 200 OK\n" \
  "Content-Type: text/plain\n" \
  "Content-Length: 12\n\n" \
  "Hello world!\n", 74);
  int new_sd;


  while (1) {
    connections = poll(pollfd, 1, 60000);
    if (!connections)
      break;
    new_sd = accept(sockfd, NULL, NULL);
    pollfd[1].fd = new_sd;
    // server = setserver[new_sd];
    // server.accept();

    pollfd[1].events = POLLIN;
    std::cout << "connection received\n";
    if (send(pollfd[1].fd, buf, 74, 0) < 0) {
      perror("send");
      close(pollfd[1].fd);
      exit(errno);
    }
    close(pollfd[1].fd);
  }

  close(pollfd[0].fd);
  close(pollfd[1].fd);
  close(sockfd);
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
