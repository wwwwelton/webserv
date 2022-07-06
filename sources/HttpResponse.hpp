#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "HttpBase.hpp"
#include "Httpform.hpp"
#include "Server.hpp"

class HttpResponse: public HttpBase {
public:
  static void handler(int fd, t_httpform const& form);
};

#endif //HTTPRESPONSE_HPP
