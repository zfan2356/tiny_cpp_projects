#pragma once

#include "util.h"
#include <sys/socket.h>

namespace webserver::utils {

class Socket {
public:
  Socket();
  Socket(int fd);
  ~Socket();

  Socket &operator=(const Socket &) = delete;
  Socket(const Socket &) = delete;

  Socket &operator=(Socket &&other) noexcept;
  Socket(Socket &&other) noexcept;

  void bind(const EndPoint &endpoint);
  void listen(int backlog = 128);
  int accept(const EndPoint &endpoint);
  int get_fd() noexcept;

private:
  int fd_;
};

} // namespace webserver::utils
