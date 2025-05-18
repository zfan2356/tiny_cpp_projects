#include "server.h"
#include "../utils/epoll.h"
#include "../utils/socket.h"
#include <netinet/in.h>
#include <unistd.h>

namespace webserver::server {
using ::webserver::utils::Epoll;
using ::webserver::utils::Socket;

using ::webserver::utils::EndPoint;

Server::Server() {
  Socket *svr_socket = new Socket();
  EndPoint endpoint("127.0.0.1", 8080);

  svr_socket->bind(endpoint);
  svr_socket->listen();

  Epoll *epoll = new Epoll();
  epoll->add_fd(svr_socket->get_fd(), 0, EPOLLIN | EPOLLLET);
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
