// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#ifndef WEBSERV_H
#define WEBSERV_H

#define PORT1 3490
#define PORT2 3491

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <utility>
#include <string>


#include "Server.hpp"
#include "ServerConfig.hpp"
#include "Utils.hpp"
#include "Pollfd.hpp"

class Server;

typedef struct addrinfo s_addrinfo;
int init(char **argv, std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds);


#endif  // WEBSERV_H
