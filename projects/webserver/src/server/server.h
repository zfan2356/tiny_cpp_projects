#pragma once

#include <cassert>
#include <cstdint>
#include <sys/socket.h>

namespace webserver::server {

class Endpoint {
public:
  Endpoint(uint32_t ip, uint16_t port);
  ~Endpoint();

  Endpoint(const Endpoint &) = delete;
  Endpoint &operator=(const Endpoint &) = delete;

private:
  uint32_t ip_;
  uint16_t port_;
};

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
