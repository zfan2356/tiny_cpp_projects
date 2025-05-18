#include "server.h"
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>

namespace webserver::server {

constexpr size_t BUFFER_SIZE = 1024;

inline Server::Server() {
  socket_ = std::make_unique<util::Socket>();
  util::EndPoint endpoint("127.0.0.1", 8080);

  socket_->bind(endpoint);
  socket_->listen();

  epoll_ = std::make_unique<util::Epoll>();
  epoll_->add_fd(socket_->get_fd(), 0, EPOLLIN | EPOLLET);
}

inline Server::~Server() {
  if (socket_->get_fd() >= 0) {
    close(socket_->get_fd());
  }
}

inline Server::Server(Server &&other) noexcept
    : socket_(std::move(other.socket_)), epoll_(std::move(other.epoll_)) {
  other.socket_ = nullptr;
  other.epoll_ = nullptr;
}

inline Server &Server::operator=(Server &&other) noexcept {
  socket_ = std::move(other.socket_);
  epoll_ = std::move(other.epoll_);
  return *this;
}

inline void handle_read(int fd) {
  char buffer[BUFFER_SIZE];
  while (true) {
    bzero(buffer, sizeof(buffer));
    ssize_t bytes = read(fd, buffer, BUFFER_SIZE);
    if (bytes > 0) {
      LOG() << "[Server] message from client fd: " << fd << "msg: " << buffer;
      write(fd, buffer, bytes);
    } else if (bytes == -1 && errno == EINTR) {
      LOG() << "[Server] client fd: " << fd << " continue reading";
      continue;
    } else if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      LOG() << "[Server] finish reading, errno: " << errno;
      break;
    } else if (bytes == 0) {
      LOG() << "[Server] EOF, client fd: " << fd << " close connection";
      close(fd);
      break;
    } else {
      LOG() << "[Server] unknown case";
    }
  }
}

void Server::start() {
  auto cb = [this](const epoll_event &event) {
    if (event.data.fd == socket_->get_fd()) {
      // new client connected
      LOG() << "[Server] Find new client, connecting...";
      auto client_addr_ = std::make_unique<util::EndPoint>();
      auto client_socket_ =
          std::make_unique<util::Socket>(this->socket_->accept(*client_addr_));
      LOG() << "[Server] new client connected: "
            << client_addr_.get()->addr_.sin_addr.s_addr;
      epoll_->add_fd(client_socket_->get_fd(), 0, EPOLLIN | EPOLLET);
    } else if (event.events & EPOLLIN) {
      // read event
      LOG() << "[Server] find read event";
      handle_read(event.data.fd);
    } else {
      LOG() << "[Server] find unknown event";
    }
  };

  while (true) {
    epoll_->wait(cb);
  }
}

} // namespace webserver::server
