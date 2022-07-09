// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 24/06/2022

#include "webserv.h"

int init(int argc,
         char** argv,
         std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds) {
  (void)argc;

  size_t i;
  ServerConfig configfiles;

  configfiles = ServerConfig(argv);

  for (i = 0; i < configfiles.size(); i++) {
    Server* tmp = new Server;
    tmp->port = htons(PORT1 + i);
    tmp->_connect(500);

    map->insert(std::make_pair(tmp->sockfd, tmp));
    pollfds->push_back(_pollfd(tmp->sockfd, POLLIN));
  }

  return i;
}
