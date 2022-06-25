// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#ifndef WEBSERV_H
#define WEBSERV_H

#define PORT1 3490
#define PORT2 3491

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <set>
#include <netdb.h>
#include <iostream>
#include <utility>
#include "Server.hpp"

#include <string>

#include "Utils.hpp"

class Server;

typedef struct addrinfo s_addrinfo;
int init(char **argv, std::map<int, Server*>* map, pollfd *pollfds);


#endif  // WEBSERV_H
