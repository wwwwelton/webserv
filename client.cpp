#include <cstddef>
#include <iostream>
#include <cstdio>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (void)
{
  struct sockaddr_in server;
  int sock_fd;

  memset(&server, 0, sizeof server);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(3492);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

  int conn_fd = connect(sock_fd, (struct sockaddr*)&server, sizeof server);
  if (conn_fd == -1) {
    perror("connect");
    exit(1);
  }

  perror("connected");

  const char *data = "GET /index.html HTTP/1.1\r\n"
                     "Host: 127.0.0.1:3492\r\n\r\n";
  int len = strlen(data);

  send(sock_fd, data, len, 0);
  // for (int i = 0; i < len; i++) {
  //   send(sock_fd, data + i, 1, 0);
  //   std::cout << "Sending a byte to the server" << std::endl;
  //   sleep(1);
  // }
  close(conn_fd);
  close(sock_fd);
  return 0;
}