//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Server.hpp"

Server::Server(void) {
  ip = INT_MAX;
  port = -1;
  root = "";
  timeout = 0;
  client_max_body_size = -1;
  redirect = std::make_pair(0, "");
  autoindex = -1;
  sockfd = DFL_SOCK_FD;
  upload = -1;
  upload_store = "";
}

Server::Server(const Server& src) {
  *this = src;
}

Server::~Server(void) {
  if (sockfd > 0)
    close(sockfd);
}

Server& Server::operator=(const Server& rhs) {
  if (this != &rhs) {
    ip = rhs.ip;
    port = rhs.port;
    server_name = rhs.server_name;
    root = rhs.root;
    index = rhs.index;
    error_page = rhs.error_page;
    timeout = rhs.timeout;
    client_max_body_size = rhs.client_max_body_size;
    log = rhs.log;
    cgi = rhs.cgi;
    redirect = rhs.redirect;
    location = rhs.location;
    autoindex = rhs.autoindex;
    sockfd = rhs.sockfd;
    upload = rhs.upload;
    upload_store = rhs.upload_store;
  }
  return (*this);
}

void Server::fill(void) {
  if (ip == INT_MAX)
    ip = inet_addr(DFL_ADDRESS);
  if (port == -1)
    port = htons(DFL_PORT);
  if (root.empty())
    root = DFL_SERVER_ROOT;
  if (index.size() == 0)
    index = String::split(DFL_SERVER_INDEX, " ");
  if (error_page.count(404) == 0)
    error_page[404] = DFL_404_PAGE;
  if (error_page.count(405) == 0)
    error_page[405] = DFL_405_PAGE;
  if (timeout == 0)
    timeout = DFL_TIMEOUT;
  if (client_max_body_size == -1)
    client_max_body_size = DFL_CLI_MAX_BODY_SIZE;
  if (autoindex == -1)
    autoindex = DFL_AUTO_INDEX;
  std::map<std::string, ServerLocation>::iterator it;
  for (it = location.begin(); it != location.end(); it++)
    it->second.fill(*this);
  if (upload == -1)
    upload = DFL_UPLOAD;
  if (upload_store.empty())
    upload_store = DFL_UPLOAD_STORE;
}

bool Server::is_invalid(void) {
  if (server_name.size() == 0) {
    error = "server_name";
    return (true);
  }
  if (location.size() == 0) {
    error = "location";
    return (true);
  }
  return (false);
}

void Server::_socket(void) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    throw ConnectException("socket");
  if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
    throw ConnectException("fcntl");
}

void Server::_bind(void) {
  struct sockaddr_in sockaddress;

  sockaddress.sin_family = AF_INET;
  sockaddress.sin_port = port;
  sockaddress.sin_addr.s_addr = ip;

  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes))
    throw ConnectException("setsockopt");
  if (bind(sockfd, (const sockaddr*)&sockaddress, sizeof(sockaddr_in)))
    throw BindException("bind", port, ip);
}

void Server::_listen(int backlog) {
  if (listen(sockfd, backlog))
    throw ConnectException("listen");
}

int Server::_connect(int backlog) {
  _socket();
  _bind();
  _listen(backlog);
  return 0;
}

void Server::print(void) {
  static size_t n = 1;

  std::cout << "====VHOST " << n << "====\n";

  in_addr t;
  t.s_addr = ip;
  std::cout << "ip: =>" << inet_ntoa(t) << "<=\n";

  std::cout << "port: =>" << ntohs(port) << "<=\n";

  for (size_t i = 0; i < server_name.size(); i++) {
    std::cout << "server name: =>" << server_name[i] << "<=\n";
  }

  std::cout << "root: =>" << root << "<=\n";

  for (size_t i = 0; i < index.size(); i++) {
    std::cout << "index: =>" << index[i] << "<=\n";
  }

  for (std::map<int, std::string>::const_iterator it = error_page.begin();
       it != error_page.end();
       it++) {
    std::cout << "error_page: =>" << it->second << "<=\n";
  }

  std::cout << "timeout: =>" << timeout << "<=\n";

  std::cout << "client_max_body_size: =>" << client_max_body_size << "<=\n";

  std::cout << "access_log: =>" << log["access_log"] << "<=\n";

  std::cout << "error_log: =>" << log["error_log"] << "<=\n";

  std::cout << "autoindex: =>" << autoindex << "<=\n";

  for (std::map<std::string, std::string>::const_iterator it = cgi.begin();
       it != cgi.end();
       it++) {
    std::cout << "cgi: =>" << it->first << "<= =>" << it->second << "<=\n";
  }

  std::cout << "redirect: =>" << redirect.first << "<= =>"
            << redirect.second << "<=\n";

  std::cout << "upload: =>" << upload << "<=\n";

  std::cout << "upload_store: =>" << upload_store << "<=\n";

  std::cout << "sockfd: =>" << sockfd << "<=\n";

  for (std::map<std::string, ServerLocation>::const_iterator
           it = location.begin();
       it != location.end();
       it++) {
    std::string index = it->first;

    std::cout << "location name: =>" << index << "<=\n";

    std::cout << "    root: =>" << location[index].root << "<=\n";

    for (size_t i = 0; i < location[index].index.size(); i++) {
      std::cout << "    index: =>" << location[index].index[i] << "<=\n";
    }

    for (size_t i = 0; i < location[index].limit_except.size(); i++) {
      std::cout << "    limit_except: =>"
                << location[index].limit_except[i] << "<=\n";
    }

    std::cout << "    client_max_body_size: =>"
              << location[index].client_max_body_size << "<=\n";

    std::cout << "    autoindex: =>" << location[index].autoindex << "<=\n";

    for (std::map<std::string, std::string>::const_iterator
             it = location[index].cgi.begin();
         it != location[index].cgi.end();
         it++) {
      std::cout << "    cgi: =>" << it->first << "<= =>"
                << it->second << "<=\n";
    }

    std::cout << "    redirect: =>" << location[index].redirect.first
              << "<= =>" << location[index].redirect.second << "<=\n";

    std::cout << "    upload: =>" << location[index].upload << "<=\n";

    std::cout << "    upload_store: =>"
              << location[index].upload_store << "<=\n";
  }

  std::cout << "\n";
  n++;
}

void Server::print_addr(std::pair<const int, Server*>& p) {
  std::ios cout_default(NULL);
  cout_default.copyfmt(std::cerr);
  Logger log(LOG_LEVEL);
  in_addr t;

  t.s_addr = p.second->ip;
  log.info()
      << "Server " << std::left << std::setw(14) << std::setfill(' ')
      << p.second->server_name[0]
      << " is listening on "
      << "http://" << inet_ntoa(t) << ":"
      << ntohs(p.second->port)
      << std::endl;
  std::cerr.copyfmt(cout_default);
}

Server::ConnectException::ConnectException(const std::string& str)
    : LoadException(str) {
  _m = "WebServ Failed to start (" + str + "): " + strerror(errno);
}

const char* Server::ConnectException::what(void) const throw() {
  return (_m.c_str());
}

Server::BindException::BindException(const std::string& str,
                                     int _port,
                                     in_addr_t _ip)
    : LoadException(str) {
  in_addr t;
  t.s_addr = _ip;
  std::stringstream ss;
  ss << " ";
  ss << inet_ntoa(t);
  ss << ":";
  ss << ntohs(_port);
  _m = "WebServ Failed to start (" + str + "): " + strerror(errno) + ss.str();
}

const char* Server::BindException::what(void) const throw() {
  return (_m.c_str());
}
