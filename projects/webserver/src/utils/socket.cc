#include "socket.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace webserver::utils {

inline Socket::Socket() : fd_(-1) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  errif(fd_ != -1, "failed to create socket");
}

inline Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

inline Socket &Socket::operator=(Socket &&other) noexcept {
  if (this != &other) {
    if (fd_ != -1) {
      close(fd_);
    }
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

inline Socket::Socket(Socket &&other) noexcept : fd_(other.fd_) {
  other.fd_ = -1;
}

inline void Socket::bind(const EndPoint &endpoint) {
  int ret = ::bind(fd_, (sockaddr *)&endpoint.addr_, endpoint.addr_len_);
  errif(ret != 0, "failed to bind socket");
}

inline void Socket::listen(int backlog) {
  int ret = ::listen(fd_, backlog);
  errif(ret == -1, "failed to listen socket");
}

inline int Socket::accept(const EndPoint &endpoint) {
  int client_fd_ =
      ::accept(fd_, (sockaddr *)&endpoint.addr_, endpoint.addr_len_);
  errif(client_fd_ == -1, "failed to accept socket");
  return client_fd_;
}

inline int Socket::get_fd() noexcept { return fd_; }

} // namespace webserver::utils
