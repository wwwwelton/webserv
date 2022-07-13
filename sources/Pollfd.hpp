//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

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
