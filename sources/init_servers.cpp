// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 24/06/2022

#include "webserv.h"

int init(int argc,
         char** argv,
         std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds) {
  size_t i;
  ServerConfig configfiles;

  if (argc != 1) {
    std::cerr << "Error: Too many arguments!\n";
    return (-1);
  }

  configfiles = ServerConfig(argv);

  for (i = 0; i < configfiles.size(); i++) {
    Server* tmp = new Server;
    tmp->_socket();
    tmp->_bind(configfiles[i].listen);
    tmp->_listen(500);

    map->insert(std::make_pair(tmp->sockfd, tmp));
    pollfds->push_back(_pollfd(tmp->sockfd, POLLIN));
  }

  return i;
}
