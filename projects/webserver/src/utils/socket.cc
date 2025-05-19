#include "socket.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace webserver::utils {

Socket::Socket() : fd_(-1) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  errif(fd_ == -1, "failed to create socket");
}

Socket::Socket(int fd) : fd_(fd) { errif(fd_ == -1, "fd is invalid"); }

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

Socket &Socket::operator=(Socket &&other) noexcept {
  if (this != &other) {
    if (fd_ != -1) {
      close(fd_);
    }
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

Socket::Socket(Socket &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }

void Socket::bind(const EndPoint &endpoint) {
  int ret = ::bind(fd_, (sockaddr *)&endpoint.addr_, endpoint.addr_len_);
  errif(ret != 0, "failed to bind socket");
}

void Socket::listen(int backlog) {
  int ret = ::listen(fd_, backlog);
  errif(ret == -1, "failed to listen socket");
}

int Socket::accept(EndPoint &endpoint) {
  int client_fd_ =
      ::accept(fd_, (sockaddr *)&endpoint.addr_, &endpoint.addr_len_);
  errif(client_fd_ == -1, "failed to accept socket");
  return client_fd_;
}

int Socket::connect(const EndPoint &endpoint) {
  int ret = ::connect(fd_, (sockaddr *)&endpoint.addr_, endpoint.addr_len_);
  errif(ret == -1, "failed to connect socket");
  return ret;
}

int Socket::get_fd() noexcept { return fd_; }

} // namespace webserver::utils
