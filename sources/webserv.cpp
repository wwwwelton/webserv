//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 21/06/2022

#include "webserv.h"

void accept_connections(std::vector<_pollfd>* pollfds, int socketfd) {
  int new_sd;

  new_sd = accept(socketfd, NULL, NULL);
  while (new_sd != -1) {
    pollfds->push_back(_pollfd(new_sd, POLLIN));
    new_sd = accept(socketfd, NULL, NULL);
  }
}

void send_messages(std::vector<_pollfd>* pollfds, int i, char *buf, char *buf2){
  int rc;
  (void)rc;
  rc = recv((*pollfds)[i].fd, buf, sizeof(buf) - 74, 0);
  rc = send((*pollfds)[i].fd, buf2, 74, 0);
  close((*pollfds)[i].fd);
  (*pollfds)[i].fd = -1;
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
  std::map<int, Server*> serverlist;
  std::map<int, int>     clientlist;
  std::vector<_pollfd>   pollfds(15);


  char buf[512000];
  char buf2[512000];
  strncpy(buf2,
          "HTTP/1.1 200 OK\n"
          "Content-Type: text/plain\n"
          "Content-Length: 12\n\n"
          "Hello world!\n", 74);


  int connections;
  int compress = false;
  init(argv, &serverlist, &pollfds);
  while (1) {
    connections =
      poll((struct pollfd*)&(*pollfds.begin()), pollfds.size(), 60000);
    std::cout << "returned connections: " << connections << '\n';
    if (connections <= 0)
      break;
    for (int i = 0, size = pollfds.size(); i < size; i++) {
      if (pollfds[i].revents == 0) {
        continue;
      }
      if (serverlist[pollfds[i].fd]) {
        std::cout << pollfds[i].fd << " socket has events\n";
        accept_connections(&pollfds, pollfds[i].fd);
      }
      else {
        std::cout << pollfds[i].fd << " client has events\n";
        send_messages(&pollfds, i, buf, buf2);
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
