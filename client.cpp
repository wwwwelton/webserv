#include <cstddef>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <netinet/in.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void print_chunk(const std::string &str, size_t start, size_t size) {
  for (size_t i = start; size > 0; size--) {
    char c = str[i++];
    if (std::isprint(c)) {
      std::cout << c;
    } else {
      if (c == '\r')
        std::cout << "\\r";
      else if (c == '\n')
        std::cout << "\\n";
      else
        std::cout << '.';
    }
  }
}

void send_string_chunks(int sock_fd, const std::string &str, int write_size) {
  int len = str.size();
  const char *data = str.c_str();

  if (write_size == 0) 
    write_size = len;
  // send(sock_fd, data, len, 0);
  int chunk_start = 0;
  for (int i = 0; chunk_start < len; i++) {
    if (chunk_start + write_size > len)
      write_size = len - chunk_start;

    std::cout << "sending chunk [";
    print_chunk(str, chunk_start, write_size);
    std::cout << "]" << std::endl;
    int result = send(sock_fd, data + chunk_start, write_size, 0);

    if (result == -1) {
      std::cout << "failed to send chunk [";
      print_chunk(str, chunk_start, write_size);
      std::cout << "]" << std::endl;
    }
    chunk_start += write_size;
    usleep(1000 * 1000 * 0.1);
    // std::string input;
    // std::getline(std::cin, input);
  }
}

int send_data(
    const std::string &port,
    int write_size,
    std::ifstream &content) {
  std::stringstream ss;
  uint16_t p;

  struct sockaddr_in server;
  int sock_fd;

  memset(&server, 0, sizeof server);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    exit(1);
  }

  server.sin_family = AF_INET;
  ss << port;
  ss >> p;

  server.sin_port = htons(p);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

  int conn_fd = connect(sock_fd, (struct sockaddr*)&server, sizeof server);
  if (conn_fd == -1) {
    std::cout << "port: " << port << std::endl;
    perror("connect");
    exit(1);
  }
  std::string data;

  while (1) {
    std::string line;
    std::getline(content, line);
    if (content.eof())
      break;
    data.append(line + "\n");
  }

  send_string_chunks(sock_fd, data, write_size);

  close(conn_fd);
  close(sock_fd);
  return (0);
}

int main (int argc, char **argv)
{

  if (argc != 3 && argc != 4) {
    std::cout
      << "Usage:\n\n"
      << argv[0] << " <port> <request-file>.http [write-size]\n\n"
      << "Where <request-file>.http is a file that contains a full HTTP request\n"
      << "and [write-size] is the size that will be used on the send() operations."
      << std::endl;
    exit(2);
  }

  /*
   * argv[1] - port
   * argv[2] - file
   * argv[3] - write-size
   * */

  std::string port = argv[1];
  int write_size = 0;
  if (argc == 4) {
    std::stringstream ss(argv[3]);
    ss >> write_size;
  }

  std::cout
    << "sending request in pieces of size " << write_size
    << " to localhost on port " << port  << "." << std::endl;

  std::ifstream content(argv[2]);
  if (!content.good()) {
    perror(argv[2]);
  }
  return send_data(port, write_size, content);
}
