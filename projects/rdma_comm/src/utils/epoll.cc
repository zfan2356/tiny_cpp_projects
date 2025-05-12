#include "epoll.h"
#include "c10/util/Exception.h"
#include <assert.h>
#include <cerrno>
#include <cstdint>
#include <memory>
#include <sys/epoll.h>

namespace rc::utils {

inline Epoll::Epoll() : fd_(epoll_create1(EPOLL_CLOEXEC)) {
  TORCH_CHECK(fd_ != -1, "failed to create epoll instance, errno=", errno,
              " errmsg=", strerror(errno))
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
  struct epoll_event *ev = new epoll_event;
  ev->events = mode;
  ev->data = data;
  TORCH_CHECK(epoll_ctl(fd_, EPOLL_CTL_ADD, fd, ev) == 0,
              "failed to add fd to epoll, errno=", errno,
              " errmsg=", strerror(errno))
}

inline void Epoll::del_fd(int fd) {
  TORCH_CHECK(epoll_ctl(fd_, EPOLL_CTL_DEL, fd, nullptr) == 0,
              "failed to delete fd from epoll, errno=", errno,
              " errmsg=", strerror(errno))
}

template <concepts::WaitCallBack CallBack>
inline void Epoll::wait(CallBack &&cb) {
  auto events = std::make_unique<epoll_event[]>(1204);
  int n = epoll_wait(fd_, events.get(), 1024, -1);
  TORCH_CHECK(n >= 0, "failed to wait for epoll events, errno=", errno,
              " errmsg=", strerror(errno));
  for (int i = 0; i < n; ++i) {
    std::forward<CallBack>(cb)(events[i]);
  }
}

} // namespace rc::utils
