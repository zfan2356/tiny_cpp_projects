#include "utils/stream_logger.h"
#include <chrono>
#include <cstdlib>
#include <gtest/gtest.h>
#include <thread>

namespace webserver::utils {
TEST(LoggerTest, Main) {
  setenv("WEBSERVER_LOG_TO_STDERR", "1", 1);
  LOG() << "Hello, World!";

  LOG() << "Hello, World2!";

  LOG() << "Hello, World3!";
}
} // namespace webserver::utils
