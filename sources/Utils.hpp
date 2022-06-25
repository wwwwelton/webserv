// Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
// Creation date: 21/06/2022

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>



struct server_config {
  int listen;
  std::string server_name;
  std::string root;
};

struct server_config* readconfig(char** argv);

#endif  // UTILS_HPP_
