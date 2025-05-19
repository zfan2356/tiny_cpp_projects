#include "epoll.h"
#include "util.h"
#include <assert.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <sys/epoll.h>

namespace webserver::utils {

Epoll::Epoll() : fd_(epoll_create1(EPOLL_CLOEXEC)) {
  std::stringstream ss;
  ss << "failed to create epoll instance, errno=" << errno
     << " errmsg=" << strerror(errno);
  errif(fd_ == -1, ss.str().c_str());
}

Epoll::~Epoll() {
  if (fd_ != -1) {
    close(fd_);
  }
}

Epoll::Epoll(Epoll &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
Epoll &Epoll::operator=(Epoll &&other) noexcept {
  if (this != &other) {
    if (fd_ != -1) {
      close(fd_);
    }
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

void Epoll::add_fd(int fd, epoll_data_t data, uint32_t mode) {
  // TODO: Can this be optimized?
  struct epoll_event *ev = new epoll_event;
  ev->events = mode;
  ev->data = data;

  int ret = epoll_ctl(fd_, EPOLL_CTL_ADD, fd, ev);

  if (ret != 0) {
    std::stringstream ss;
    ss << "failed to add fd to epoll, errno=" << errno
       << " errmsg=" << strerror(errno);
    errif(ret != 0, ss.str().c_str());
  }
}

void Epoll::del_fd(int fd) {
  int ret = epoll_ctl(fd_, EPOLL_CTL_DEL, fd, nullptr);
  std::stringstream ss;
  ss << "failed to delete fd from epoll, errno=" << errno
     << " errmsg=" << strerror(errno);
  errif(ret != 0, ss.str().c_str());
}

} // namespace webserver::utils
