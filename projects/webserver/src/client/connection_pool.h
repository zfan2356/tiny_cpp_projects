#pragma once

#include "utils/util.h"
#include <functional>
#include <memory>
#include <variant>

namespace webserver::client {

namespace util = ::webserver::utils;

struct ConnectionFailed {
  int errno_;
  std::string message_;

  std::string str() const {
    std::ostringstream oss;
    oss << "[Client] connection failed, errno=" << errno_
        << ", errmsg=" << message_;
    return oss.str();
  }
};

template <typename ConnectionState> class ConnectionPool {
  using CallBack = std::function<void(
      std::variant<ConnectionFailed, std::pair<int, ConnectionState *>>)>;
  struct Context {
    int fd_;
    utils::EndPointV2 ep_;
    ConnectionState *state_;
    std::vector<CallBack> callbacks_;

    void on_error() {}
    void on_established() {}

} public
    : explicit ConnectionPool(const std::vector<util::EndPointV2> &endpoints,
                              CallBack callback) {
  }
};

} // namespace webserver::client
