//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include <netdb.h>
#include <iostream>
#include "webserv.h"

#define PORT "3490"

int main(int argc, char **argv) {
  struct sockaddr_in sockaddress;

  int  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  if (sockfd == -1) {
    perror("socket");
    exit(errno);
  }
  sockaddress.sin_family = AF_INET;
  sockaddress.sin_port = htons(3490);
  sockaddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  std::cout << sizeof(sockaddr) << " " << sizeof(sockaddr_in) << "\n";
  if (bind(sockfd, (const sockaddr *)&sockaddress, sizeof(sockaddr_in))) {
    perror("bind");
    exit(errno);
  }
  if (listen(sockfd, 5)) {
    perror("listen");
    exit(errno);
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
  std::cout << "Socket ready: fd = " << sockfd << '\n';
}
