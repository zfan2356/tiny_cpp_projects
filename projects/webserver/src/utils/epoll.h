#pragma once

#include "util.h"
#include <memory>
#include <sstream>
#include <sys/epoll.h>
#include <type_traits>
#include <unistd.h>

namespace webserver::utils {

namespace concepts {
template <typename T>
concept WaitCallBack = std::is_invocable_r_v<void, T, const epoll_event &>;
}

class Epoll {
public:
  Epoll();
  ~Epoll();

  Epoll(const Epoll &) = delete;
  Epoll &operator=(const Epoll &) = delete;

  Epoll(Epoll &&) noexcept;
  Epoll &operator=(Epoll &&) noexcept;

  void add_fd(int fd, uint64_t u64, uint32_t events = EPOLLIN) {
    epoll_data_t data;
    data.u64 = u64;
    add_fd(fd, data, events);
  }
  void add_fd(int fd, epoll_data_t data, uint32_t events = EPOLLIN);

  void del_fd(int fd);

  template <concepts::WaitCallBack CallBack> void wait(CallBack &&callback) {
    auto events = std::make_unique<epoll_event[]>(1024);
    int n = epoll_wait(fd_, events.get(), 1024, -1);
    if (n < 0) {
      std::stringstream ss;
      ss << "failed to wait for epoll events, errno=" << errno
         << " errmsg=" << strerror(errno);
      errif(n < 0, ss.str().c_str());
    }
    for (int i = 0; i < n; ++i) {
      std::forward<CallBack>(callback)(events[i]);
    }
  }

  int get_fd() const noexcept { return fd_; }

private:
  int fd_{-1};
};
} // namespace webserver::utils
