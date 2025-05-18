#include "epoll.h"
#include "util.h"
#include <assert.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <sys/epoll.h>

namespace webserver::utils {

inline Epoll::Epoll() : fd_(epoll_create1(EPOLL_CLOEXEC)) {
  std::stringstream ss;
  ss << "failed to create epoll instance, errno=" << errno
     << " errmsg=" << strerror(errno);
  errif(fd_ != -1, ss.str().c_str());
}

inline Epoll::~Epoll() {
  if (fd_ != -1) {
    close(fd_);
  }
}

inline Epoll::Epoll(Epoll &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
inline Epoll &Epoll::operator=(Epoll &&other) noexcept {
  if (this != &other) {
    if (fd_ != -1) {
      close(fd_);
    }
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

inline void Epoll::add_fd(int fd, epoll_data_t data, uint32_t mode) {
  // TODO: Can this be optimized?
  struct epoll_event *ev = new epoll_event;
  ev->events = mode;
  ev->data = data;

  int ret = epoll_ctl(fd_, EPOLL_CTL_ADD, fd, ev);

  if (ret != 0) {
    std::stringstream ss;
    ss << "failed to add fd to epoll, errno=" << errno
       << " errmsg=" << strerror(errno);
    errif(ret == 0, ss.str().c_str());
  }
}

inline void Epoll::del_fd(int fd) {
  int ret = epoll_ctl(fd_, EPOLL_CTL_DEL, fd, nullptr);
  std::stringstream ss;
  ss << "failed to delete fd from epoll, errno=" << errno
     << " errmsg=" << strerror(errno);
  errif(ret == 0, ss.str().c_str());
}

template <concepts::WaitCallBack CallBack>
inline void Epoll::wait(CallBack &&cb) {
  auto events = std::make_unique<epoll_event[]>(1204);
  int n = epoll_wait(fd_, events.get(), 1024, -1);
  if (n < 0) {
    std::stringstream ss;
    ss << "failed to wait for epoll events, errno=" << errno
       << " errmsg=" << strerror(errno);
    errif(n < 0, ss.str().c_str());
  }
  for (int i = 0; i < n; ++i) {
    std::forward<CallBack>(cb)(events[i]);
  }
}

} // namespace webserver::utils
