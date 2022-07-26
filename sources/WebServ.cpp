//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "WebServ.hpp"

s_request::s_request(void)
    : server(NULL), request_parser(NULL) {}
s_request::s_request(Server *_server, int fd)
    : server(_server), request_parser(new RequestParser(fd)) {}

Logger WebServ::log = WebServ::init_log();
Logger WebServ::init_log(void) {
  Logger logger(LOG_LEVEL);
  return logger;
}

WebServ::~WebServ(void) {
  log.debug() << "WebServ destructor called\n";
  std::map<int, Server *>::iterator it = serverlist.begin();
  std::map<int, Server *>::iterator ite = serverlist.end();
  for (; it != ite; it++) {
    delete it->second;
  }
  {
    std::vector<_pollfd>::iterator it = pollfds.begin();
    std::vector<_pollfd>::iterator ite = pollfds.end();
    for (; it != ite; it++) {
      if (serverlist.count(it->fd))
        continue;
      delete clientlist[it->fd].request_parser;
      delete clientlist[it->fd].response;
      clientlist[it->fd].request_parser = NULL;
      clientlist[it->fd].server = NULL;
      it->fd = -1;
    }
  }
}

WebServ::WebServ(int argc, char **argv) {
  log.info() << "WebServ Initializing\n";
  size_t i;

  init_signals(this);
  log.info() << "WebServ Signals initialized\n";

  validate_input(argc, argv);
  conf.parse(argv[1]);
  log.info() << "WebServ Loaded " << argv[1] << "\n";

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);

  for (i = 0; i < conf.size(); i++) {
    Server *srv = new Server(conf[i]);
    srv->_connect(conf.backlog);
    serverlist.insert(std::make_pair(srv->sockfd, srv));
    pollfds.push_back(_pollfd(srv->sockfd, POLLIN));
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
  int _fd;

  log.info() << "Events detected in socket " << pollfds[i].fd << "\n";
  _fd = accept(host->sockfd, NULL, NULL);
  while (_fd != -1) {
    clientlist[_fd].server = host;
    int max_body_size = host->client_max_body_size;
    clientlist[_fd].request_parser = new RequestParser(_fd, max_body_size);
    clientlist[_fd].response = new Response(NULL, host);
    pollfds.push_back(_pollfd(_fd, POLLIN));
    log.info() << host->server_name[0]
               << " accepted connection of client "
               << _fd << "\n";
    _fd = accept(host->sockfd, NULL, NULL);
  }
}

void WebServ::end_connection(int i) {
  int fd = pollfds[i].fd;

  delete clientlist[fd].request_parser;
  delete clientlist[fd].response;
  clientlist[fd].request_parser = NULL;
  clientlist[fd].server = NULL;
  close(pollfds[i].fd);
  log.info() << "Connection closed with client " << pollfds[i].fd << "\n";
  pollfds[i].fd = -1;
  compress = true;
}

void WebServ::_receive(int i) {
  int fd = pollfds[i].fd;
  RequestParser &parser = *clientlist[fd].request_parser;

  try {
    parser.parse();
    if (parser.finished)
      pollfds[i].events = POLLIN | POLLOUT;
  } catch (std::exception &e) {
    WebServ::log.error()
        << "exception caught while tokenizing request: "
        << e.what() << std::endl;
    end_connection(i);
    return;
  }
}

void WebServ::_respond(int i) {
  int fd = pollfds[i].fd;

  RequestParser &parser = *clientlist[fd].request_parser;
  Response &response = *clientlist[fd].response;

  if (response.inprogress) {
    response.assemble(response.response_path);
  }
  else if (parser.finished) {
    response.set_request(&parser.get_request());
    response.process();
    response._send(fd);
  }
  if (response.finished)
    end_connection(i);
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
