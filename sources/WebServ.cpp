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
  for (; it != ite; it++)
    delete it->second;
}

WebServ::WebServ(int argc, char **argv) {
  log.info() << "WebServ Initializing\n";
  size_t i;

  _init_signals();
  log.info() << "WebServ Signals Initialized\n";

  _validate_input(argc, argv);
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
  int new_sd;

  log.info() << "Events detected in socket " << pollfds[i].fd << "\n";
  new_sd = accept(host->sockfd, NULL, NULL);
  while (new_sd != -1) {
    clientlist[new_sd].request_parser = new RequestParser(new_sd);
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
  Response req_handler;

  RequestParser &parser = *clientlist[fd].request_parser;
  try {
    parser.parse();
  } catch (std::exception &e) {
    WebServ::log.error()
        << "exception caught while tokenizing request: "
        << e.what() << std::endl;
    return;
  }

  if (parser.finished) {
    Request &ptr = parser.get_request();
    req_handler = Response(ptr, clientlist[fd].server);
    req_handler.process();
    req_handler._send(fd);
    delete clientlist[fd].request_parser;
    clientlist[fd].request_parser = NULL;
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

void WebServ::_validate_input(int argc, char **argv) {
  if (argc < 2)
    throw LoadException("no file provided");
  if (argc > 2)
    throw LoadException("too many arguments");
  if (argv[1][0] == '\0')
    throw LoadException("no file provided");
  std::string file(argv[1]);
  std::string::size_type pos = file.find_last_of(".");
  if (pos == std::string::npos)
    throw LoadException("invalid file extension");
  std::string ext = file.substr(pos);
  if (ext != CFG_FILE_EXT)
    throw LoadException("invalid file extension");
  std::ifstream ifs(argv[1], std::ios::binary | std::ios::ate);
  if (ifs.fail()) {
    ifs.close();
    throw LoadException(strerror(errno));
  }
  if (ifs.tellg() > (CFG_FILE_MAX_SIZE * 1000)) {
    ifs.close();
    throw LoadException("file too large");
  }
}

void WebServ::_init_signals(void) {
  void *func = NULL;
  func = reinterpret_cast<void *>(sighandler);
  std::signal(SIGINT, reinterpret_cast<__sighandler_t>(func));
  std::signal(SIGQUIT, reinterpret_cast<__sighandler_t>(func));
  sighandler(0, this);
}
