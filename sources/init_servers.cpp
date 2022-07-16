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
  Logger logger(LVL_DEBUG);

  if (argc != 2) {
    logger.error() << "please provide one config file!\n";
    exit(1);
  } else {
    logger.debug() << "config file: " << argv[1] << "\n";
  }
  configs = Config(argv[1]);

  for (i = 0; i < configs.size(); i++) {
    configs[i]->_connect(configs.backlog);

    map->insert(std::make_pair(configs[i]->sockfd, configs[i]));
    pollfds->push_back(_pollfd(configs[i]->sockfd, POLLIN));
  }

  return i;
}
