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
{
  std::vector<_pollfd>:: iterator it = pollfds.begin();
  std::vector<_pollfd>:: iterator ite = pollfds.end();
  for (; it != ite; it++) {
    if (serverlist.count(it->fd))
      continue;
    delete clientlist[it->fd].request_parser;
    clientlist[it->fd].request_parser = NULL;
    clientlist[it->fd].server = NULL;
    it->fd = -1;
  }
}
}

WebServ::WebServ(int argc, char **argv) {
  log.info() << "WebServ Initializing\n";
  size_t i;

  _init_signals();
  log.info() << "WebServ Signals Initialized\n";

  if (_valid_input(argc, argv)) {
    configs = Config(argv[1]);
  }
  log.info() << "WebServ Loaded " << argv[1] << "\n";

  conn = 0;
  compress = false;
  clientlist.reserve(1024);
  clientlist.resize(1024);

  for (i = 0; i < configs.size(); i++) {
    Server *srv = new Server(configs[i]);
    srv->_connect(configs.backlog);
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
    clientlist[new_sd].server = host;
    int max_body_size = host->client_max_body_size;
    clientlist[new_sd].request_parser = new RequestParser(new_sd, max_body_size);
    pollfds.push_back(_pollfd(new_sd, POLLIN));
    log.info() << host->server_name[0]
               << " accepted connection of client "
               << new_sd << "\n";
    new_sd = accept(host->sockfd, NULL, NULL);
  }
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
    return;
  }
}

void WebServ::_respond(int i) {
  int fd = pollfds[i].fd;
  RequestParser &parser = *clientlist[fd].request_parser;

  if (parser.finished) {
    Response req_handler;
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

bool WebServ::_valid_input(int argc, char **argv) {
  if (argc < 2) {
    log.error() << "Failed to read config file: No file provided!\n";
    exit(1);
  }
  if (argc > 2) {
    log.error() << "Failed to read config file: Too many arguments!\n";
    exit(1);
  }
  if (argv[1][0] == '\0') {
    log.error() << "Failed to read config file: No file provided!\n";
    exit(1);
  }
  std::string file(argv[1]);
  std::string::size_type pos = file.find_last_of(".");
  if (pos == std::string::npos) {
    log.error() << "Failed to read config file: Invalid file extension!\n";
    exit(1);
  }
  std::string ext = file.substr(pos);
  if (ext != CFG_FILE_EXT) {
    log.error() << "Failed to read config file: Invalid file extension!\n";
    exit(1);
  }
  std::ifstream ifs(argv[1], std::ios::binary | std::ios::ate);
  if (ifs.fail()) {
    log.error() << "Failed to read config file: " << strerror(errno) << "!\n";
    exit(1);
  }
  if (ifs.tellg() > (CFG_FILE_MAX_SIZE * 1000)) {
    log.error() << "Failed to read config file: File too large!\n";
    exit(1);
  }
  ifs.close();
  return (true);
}

void WebServ::_init_signals(void) {
  void *func = NULL;
  func = reinterpret_cast<void *>(sighandler);
  std::signal(SIGINT, reinterpret_cast<__sighandler_t>(func));
  std::signal(SIGQUIT, reinterpret_cast<__sighandler_t>(func));
  sighandler(0, this);
}
