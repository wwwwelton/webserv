//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 26/06/2022

#include "Pollfd.hpp"

_pollfd::_pollfd(void): fd(0), events(0), revents(0) { }
_pollfd::_pollfd(int _fd): fd(_fd), events(0), revents(0) { }
_pollfd::_pollfd(int _fd, short int _events)
  : fd(_fd), events(_events), revents(0) { }
