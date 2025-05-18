#pragma once

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

  template <concepts::WaitCallBack CallBack> void wait(CallBack &&callback);

private:
  int fd_{-1};
};
} // namespace webserver::utils
