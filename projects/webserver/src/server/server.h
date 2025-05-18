#pragma once

#include <cstdint>
#include <memory>
#include <sys/socket.h>
#include <utils/epoll.h>
#include <utils/socket.h>
#include <utils/stream_logger.h>

namespace webserver::server {

namespace util = ::webserver::utils;

class Server {
public:
  Server();
  ~Server();

  Server(const Server &) = delete;
  Server &operator=(const Server &) = delete;

  Server(Server &&) noexcept;
  Server &operator=(Server &&) noexcept;

  void start();

private:
  std::unique_ptr<util::Socket> socket_;
  std::unique_ptr<util::Epoll> epoll_;
};
} // namespace webserver::server
