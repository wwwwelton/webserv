// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 24/06/2022

#include "webserv.h"

int init(int argc,
         char** argv,
         std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds) {
  (void)argc;

  size_t i;
  Config configs;

  configs = Config(argv[1]);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    map->insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds->push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }

  return i;
}
