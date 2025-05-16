#pragma once

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <string_view>

namespace webserver::utils {

template <typename T> class BlockingQueue {
public:
  BlockingQueue() : should_stop_(false) {}
  ~BlockingQueue() = default;

  void push(T v) {
    std::unique_lock<std::mutex> lock(mtx);
    if (should_stop_)
      return;
    q_.push(std::move(v));
    not_empty.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mtx);
    while (q_.empty() && !should_stop_) {
      not_empty.wait(lock);
    }
    if (q_.empty()) {
      return T(); // Return empty value when stopped
    }
    T v = std::move(q_.front());
    q_.pop();
    return v;
  }

  void stop() {
    std::unique_lock<std::mutex> lock(mtx);
    should_stop_ = true;
    not_empty.notify_all();
  }

  bool empty() {
    std::unique_lock<std::mutex> lock(mtx);
    return q_.empty();
  }

private:
  std::mutex mtx;
  std::queue<T> q_;
  std::condition_variable not_empty;
  std::atomic<bool> should_stop_;
};

using LogHandler = std::function<void(std::string_view)>;

struct LogStatus {
  bool log_to_stderr_;
  int log_file_;

  bool enable() const { return log_to_stderr_ || log_file_ != -1; }
};

inline const LogStatus &get_log_status() {
  auto log_to_stderr = []() -> bool {
    auto e = getenv("WEBSERVER_LOG_TO_STDERR");
    return e != nullptr && strcmp(e, "1") == 0;
  };

  auto log_to_file = []() -> int {
    auto log_file_ = getenv("WEBSERVER_LOG_FILE");
    if (log_file_ == nullptr) {
      return -1;
    }
    int fd = open(log_file_, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
    if (fd == -1) {
      std::cerr << "[webserver::utils] Failed to open log file: " << log_file_
                << std::endl;
      return -1;
    }
    return fd;
  };

  static LogStatus status{.log_to_stderr_ = log_to_stderr(),
                          .log_file_ = log_to_file()};
  return status;
}

inline void stderr_handler(std::string_view msg) { std::cerr << msg << "\n"; }

inline void file_handler(std::string_view msg) {
  auto &s = get_log_status();
  while (!msg.empty()) {
    auto written = write(s.log_file_, msg.data(), msg.size());
    if (written == -1) {
      std::cerr << "[webserver::utils] Failed to write to log file"
                << std::endl;
      break;
    }
    msg.remove_prefix(written);
  }
}

class Logger {
public:
  Logger()
      : worker_([this]() {
          while (true) {
            auto log = queue_.pop();
            if (log.empty() && queue_.empty()) {
              break; // Exit when queue is empty and stopped
            }
            for (auto &handle : handlers_) {
              handle(log);
            }
          }
        }) {}
  ~Logger() {
    queue_.stop();
    if (worker_.joinable()) {
      worker_.join();
    }
  }

  void log(std::string msg) { queue_.push(std::move(msg)); }

  void append_handler(LogHandler handler) {
    handlers_.push_back(std::move(handler));
  }

private:
  std::vector<LogHandler> handlers_;
  BlockingQueue<std::string> queue_;
  std::thread worker_;
};

class StreamLogger {
public:
  StreamLogger(const char *file, int line) {
    oss_ << file << ": " << line << ": ";
  }

  ~StreamLogger() {
    oss_ << std::endl;
    log(oss_.str());
  }

  std::ostream &stream() { return oss_; }

private:
  static void log(std::string msg) {
    static Logger logger;
    static std::once_flag init_once;
    std::call_once(init_once, [&]() {
      const auto &status = get_log_status();
      if (status.log_to_stderr_) {
        logger.append_handler(stderr_handler);
      }
      if (status.log_file_ != -1) {
        logger.append_handler(file_handler);
      }
    });
    logger.log(msg);
  }
  std::ostringstream oss_;
};

struct LogMessageVoidify {
  constexpr void operator&(std::ostream &) const noexcept {}
};

} // namespace webserver::utils

#define LOG()                                                                  \
  static_cast<void>(0),                                                        \
      !(::webserver::utils::get_log_status().enable())                         \
          ? (void)(0)                                                          \
          : ::webserver::utils::LogMessageVoidify() &                          \
                ::webserver::utils::StreamLogger(__FILE__, __LINE__).stream()
