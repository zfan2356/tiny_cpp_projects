#include "utils/stream_logger.h"
#include <array>
#include <cstdlib>
#include <gtest/gtest.h>
#include <thread>

namespace webserver::utils {
TEST(LoggerTest, Main) {
  setenv("WEBSERVER_LOG_TO_STDERR", "1", 1);
  LOG() << "Hello, World!";

  LOG() << "Hello, World2!";

  LOG() << "Hello, World3!";

  std::array<char, 1024> buffer;
  std::cout << sizeof(buffer) << std::endl;

  char buf[1024];
  std::cout << sizeof(buf) << std::endl;

  bzero(buf, sizeof(buf));
  std::cout << sizeof(buf) << std::endl;
}
} // namespace webserver::utils
