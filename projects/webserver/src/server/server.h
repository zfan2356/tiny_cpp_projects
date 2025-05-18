#pragma once

#include <cassert>
#include <cstdint>
#include <sys/socket.h>

namespace webserver::server {
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
  int socket_fd_{-1};
};
} // namespace webserver::server
