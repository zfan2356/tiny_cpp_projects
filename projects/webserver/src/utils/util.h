#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>

namespace webserver::utils {

inline void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}

struct EndPointV2 {
  uint32_t ip_;
  uint16_t port_;

  bool operator==(const EndPointV2 &other) const {
    return ip_ == other.ip_ && port_ == other.port_;
  }

  struct Hasher {
    size_t operator()(const EndPointV2 &endpoint) const {
      return std::hash<uint32_t>()(endpoint.ip_) ^
             std::hash<uint16_t>()(endpoint.port_);
    }
  };
};

inline std::ostream &operator<<(std::ostream &os, const EndPointV2 &endpoint) {
  char ip_str[INET_ADDRSTRLEN] = {};
  inet_ntop(AF_INET, &endpoint.ip_, ip_str, INET_ADDRSTRLEN);
  os << "{EP: " << ip_str << ":" << endpoint.port_ << "}";
  return os;
}

class EndPoint {
public:
  struct sockaddr_in addr_;
  socklen_t addr_len_;

  EndPoint() : addr_len_(sizeof(addr_)) { bzero(&addr_, sizeof(addr_)); }

  EndPoint(const char *ip, uint16_t port) {
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_len_ = sizeof(addr_);
  }

  ~EndPoint() = default;
};

} // namespace webserver::utils
