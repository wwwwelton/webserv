//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "WebServ.hpp"

s_request::s_request(void)
: server(NULL), request_parser(NULL) {
  timestamp = WebServ::get_time_in_ms();
}
s_request::s_request(Server *_server, int fd)
: server(_server), request_parser(new RequestParser(fd)) {
  timestamp = WebServ::get_time_in_ms();
}

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
  conf.load(argv[1]);
  log.info() << "WebServ Loaded " << argv[1] << "\n";

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);

  for (i = 0; i < conf.size(); i++) {
    Server *srv = new Server(conf[i]);
    srv->_connect(conf.backlog);
    std::map<std::string, ServerLocation>::iterator it = srv->location.begin();
    for (; it != srv->location.end(); it++) {
      if (it->first.find('/') != std::string::npos &&
          it->first.at(it->first.size() - 1) != '/') {
        srv->location[it->first + "/"] = srv->location[it->first];
        srv->location[it->first + "/"].root.push_back('/');
      }
    }
    serverlist.insert(std::make_pair(srv->sockfd, srv));
    pollfds.push_back(_pollfd(srv->sockfd, POLLIN));
  }
  log.info() << "WebServ initialized 🚀" << std::endl;
  std::map<int, Server *>::iterator iter = serverlist.begin();
  for (; iter != serverlist.end(); iter++) {
    log.info()
      << "Server " << iter->second->server_name[0]
      << " is listening on port " << ntohs(iter->second->port)
      << std::endl;
  }
}

bool WebServ::timed_out(int i) {
  size_t time_now;
  int fd = pollfds[i].fd;

  time_now = WebServ::get_time_in_ms();
  WebServ::log.error() << (time_now - clientlist[fd].timestamp) << "\n";
  if (time_now - clientlist[fd].timestamp > 1000) {
    WebServ::log.info() << "Client " << fd << " timed out after "
                        << (time_now - clientlist[fd].timestamp) << " ms\n";
    return true;
  }
  return false;
}

void WebServ::purge_timeouts(void) {
  size_t time_now;

  time_now = WebServ::get_time_in_ms();
  std::vector<req>::iterator it = clientlist.begin();
  for (; it != clientlist.end(); it++) {
    if (time_now - it->timestamp > 300) {
      end_connection(it->request_parser->fd);
      compress = true;
    }
  }
}

size_t WebServ::get_time_in_ms(void) {
  struct timeval time_now;

  gettimeofday(&time_now, NULL);
  return ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
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
    clientlist[_fd].timestamp = get_time_in_ms();
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
    response.assemble_followup();
    response._send(fd);
  }
  else if (parser.finished) {
    response.set_request(&parser.get_request());
    response.process();
    response._send(fd);
  }
  if (response.finished) {
    pollfds[i].events = POLLIN;
    parser.reset();
    response.reset();
    // end_connection(i);
  }
}

void WebServ::purge_conns(void) {
  // TODO(VLN37): see if necessary to remove timeouts here
  // purge_timeouts();
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
