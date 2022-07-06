#ifndef HTTPBASE_HPP
# define HTTPBASE_HPP

class HttpBase {
protected:
  static char buffer_req[512000];
  static char buffer_resp[512000];
};

#endif // HTTPBASE_HPP
