#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <type_traits>

namespace rc::utils {

namespace concepts {
template <typename T>
concept WaitCallBack = std::is_invocable_r_v<void, T, const epoll_event &>;
}

class Epoll {
public:
  Epoll();
  ~Epoll();

  Epoll(const &Epoll) = delete;
}
} // namespace rc::utils
