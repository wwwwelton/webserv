// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#ifndef WEBSERV_H
#define WEBSERV_H

#define PORT1 3492
#define PORT2 3493

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "HttpBase.hpp"
#include "HttpHandler.hpp"
#include "HttpRequest.hpp"
#include "Pollfd.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "defines.hpp"

class Server;

typedef struct addrinfo s_addrinfo;

int init(int argc, char** argv, std::map<int, Server*>* map,
         std::vector<_pollfd>* pollfds);

#endif  // WEBSERV_H
