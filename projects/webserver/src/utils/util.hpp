#pragma once

#include <cerrno>
#include <cstdio>
#include <cstdlib>

namespace webserver::utils {

void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}

} // namespace webserver::utils
