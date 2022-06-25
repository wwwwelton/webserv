// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Copyright (c) 2022 Welton Leite, wleite. All rights reserved.
// Creation date: 24/06/2022

#include "Utils.hpp"

struct server_config** readconfig(char** argv) {
  (void)argv;
  struct server_config** ret = new server_config*[3];
  ret[0] = new server_config;
  ret[0]->listen = PORT1;
  ret[0]->root = "/";
  ret[0]->server_name = "Server1";
  ret[1] = new server_config;
  ret[1]->listen = PORT2;
  ret[1]->root = "/";
  ret[1]->server_name = "Server2";
  ret[2] = new server_config;
  ret[2] = NULL;
  return (ret);
}
