//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "WebServ.hpp"

s_request::s_request(void)
: server(NULL), request(NULL) { }
s_request::s_request(Server *_server, int fd)
: server(_server), request(new Request(fd)) { }

Logger WebServ::log = WebServ::init_log();
Logger WebServ::init_log(void) {
  Logger logger(LOG_LEVEL);
  return logger;
}

WebServ::~WebServ(void) {
  log.debug() << "WebServ destructor called\n";
  std::map<int, Server *>::iterator it = serverlist.begin();
  std::map<int, Server *>::iterator ite = serverlist.end();
  for (; it != ite; it++)
    delete it->second;
}

WebServ::WebServ(int argc, char **argv) {
  log.info() << "WebServ Initializing\n";
  size_t i;

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);

  if (argc != 2) {
    log.error() << "Please provide one config file!\n";
    exit(1);
  } else {
    log.info() << "WebServ Loaded " << argv[1] << "\n";
  }
  configs = Config(argv[1]);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    serverlist.insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds.push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }
  log.info() << "WebServ initialized. Listening..." << std::endl;
}

int WebServ::_poll(void) {
  conn = poll((struct pollfd *)&(*pollfds.begin()), pollfds.size(), -1);
  log.info() << "returned connections: " << conn << '\n';
  return conn;
}

void WebServ::_accept(int i) {
  Server *host = serverlist[pollfds[i].fd];
  int new_sd;

  log.info() << "Events detected in socket " << pollfds[i].fd << "\n";
  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    clientlist[new_sd].request = new Request(new_sd);
    clientlist[new_sd].server = host;
    pollfds.push_back(_pollfd(new_sd, POLLIN));
    log.info() << host->server_name[0]
               << " accepted connection of client "
               << new_sd << "\n";
    new_sd = accept(host->sockfd, NULL, NULL);
  }
}

void WebServ::_respond(int i) {
  int fd = pollfds[i].fd;
  Request *ptr;

  ptr = clientlist[fd].request->receive(fd);
  if (ptr->finished) {
    Response req_handler(*ptr, clientlist[fd].server);
    req_handler.process();
    req_handler._send(fd);
    delete clientlist[fd].request;
    clientlist[fd].request = NULL;
    clientlist[fd].server = NULL;
    close(pollfds[i].fd);
    log.info() << "Connection closed with client " << pollfds[i].fd << "\n";
    pollfds[i].fd = -1;
    compress = true;
  }
}

void WebServ::purge_conns(void) {
  std::vector<_pollfd>::iterator it = pollfds.begin();
  std::vector<_pollfd>::iterator ite = pollfds.end();
  for (; it != ite; it++) {
    if (it->fd == -1) {
      pollfds.erase(it);
      ite--;
      if (it == ite)
        break;
    }
  }
}
