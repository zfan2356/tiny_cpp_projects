#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace webserver::utils {

void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}

class EndPoint {
public:
  struct sockaddr_in addr_;
  socklen_t addr_len_;

  EndPoint() : addr_len_(sizeof(addr_)) { bzero(&addr_, sizeof(addr_)); }

  EndPoint(const std::string &ip, uint16_t port) {
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_len_ = sizeof(addr_);
  }

  ~EndPoint() = default;
};

} // namespace webserver::utils
