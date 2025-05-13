#include "server.h"
#include <netinet/in.h>
#include <unistd.h>

namespace webserver::server {
Server::Server() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  assert(socket_fd_ >= 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(8080);

  int ret =
      bind(socket_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret == 0);

  ret = listen(socket_fd_, 1024);
  assert(ret == 0);
}

Server::~Server() {
  if (socket_fd_ >= 0) {
    close(socket_fd_);
  }
}

Server::Server(Server &&other) noexcept : socket_fd_(other.socket_fd_) {
  other.socket_fd_ = -1;
}

Server &Server::operator=(Server &&other) noexcept {
  socket_fd_ = other.socket_fd_;
  other.socket_fd_ = -1;
  return *this;
}

void Server::start() {
  while (true) {
  }
}

} // namespace webserver::server
