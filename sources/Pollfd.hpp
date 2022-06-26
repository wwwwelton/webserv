//Copyright (c) 2022 João Rodriguez A.K.A. VLN37. All rights reserved.
//Creation date: 26/06/2022

#ifndef _POLLFD_HPP
# define _POLLFD_HPP

class _pollfd {
public:
  _pollfd();
  _pollfd(int _fd);
  _pollfd(int _fd, short int _events);

  int       fd;
  short int events;
  short int revents;
};

#endif // _POLLFD_HPP
