#pragma once

#include "utils/socket.h"
#include "utils/util.h"
#include <memory>
#include <string>

namespace webserver::client {

namespace util = ::webserver::utils;

class Client {
public:
  Client();
  ~Client();

  Client(const Client &) = delete;
  Client &operator=(const Client &) = delete;

  Client(Client &&) noexcept;
  Client &operator=(Client &&) noexcept;

  void connect(const util::EndPoint &endpoint);
  void send(std::string msg);
  std::string recv();

private:
  std::unique_ptr<util::Socket> socket_;
};

} // namespace webserver::client
