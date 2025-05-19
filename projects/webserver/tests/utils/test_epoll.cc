#include "utils/epoll.h"
#include "utils/socket.h"
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

namespace webserver::utils {
namespace {

class EpollTest : public ::testing::Test {
protected:
  void SetUp() override {
    server_socket_ = std::make_unique<Socket>();
    EndPoint endpoint("127.0.0.1", 12345);
    server_socket_->bind(endpoint);
    server_socket_->listen(5);
  }

  void TearDown() override {}

  std::unique_ptr<Socket> server_socket_;
};

// create epoll test
TEST_F(EpollTest, CreateEpoll) {
  Epoll epoll;
  EXPECT_NE(epoll.get_fd(), -1);
}

// add and remove fd test
TEST_F(EpollTest, AddAndRemoveFd) {
  Epoll epoll;
  int fd = server_socket_->get_fd();

  epoll.add_fd(fd, 0, EPOLLIN);

  epoll.del_fd(fd);
}

// wait for events test
TEST_F(EpollTest, WaitForEvents) {
  Epoll epoll;
  int server_fd = server_socket_->get_fd();

  epoll.add_fd(server_fd, 0, EPOLLIN);

  std::thread client_thread([]() {
    sleep(1);
    auto client = std::make_unique<Socket>();
    EndPoint endpoint("127.0.0.1", 12345);
    client->connect(endpoint);
  });

  bool event_received = false;
  epoll.wait([&event_received](const epoll_event &ev) {
    event_received = true;
    EXPECT_EQ(ev.events, EPOLLIN);
  });

  client_thread.join();
  EXPECT_TRUE(event_received);
}

// move semantics test
TEST_F(EpollTest, MoveSemantics) {
  Epoll epoll1;
  int fd = epoll1.get_fd();

  Epoll epoll2(std::move(epoll1));
  EXPECT_EQ(epoll1.get_fd(), -1);
  EXPECT_EQ(epoll2.get_fd(), fd);

  Epoll epoll3;
  epoll3 = std::move(epoll2);
  EXPECT_EQ(epoll2.get_fd(), -1);
  EXPECT_EQ(epoll3.get_fd(), fd);
}

} // namespace
} // namespace webserver::utils
