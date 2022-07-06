#include "HttpResponse.hpp"

void HttpResponse::handler(int fd, t_httpform const& form) {
  send(fd, form.body, form.bodysize, 0);
}

