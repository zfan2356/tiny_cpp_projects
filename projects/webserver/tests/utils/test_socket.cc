#include "utils/socket.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace webserver::utils {
namespace {

class SocketTest : public ::testing::Test {
protected:
  void SetUp() override {
    endpoint_.addr_.sin_family = AF_INET;
    endpoint_.addr_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    endpoint_.addr_.sin_port = htons(12345);
    endpoint_.addr_len_ = sizeof(endpoint_.addr_);
  }

  EndPoint endpoint_;
};

TEST_F(SocketTest, CreateSocket) {
  Socket socket;
  EXPECT_NE(socket.get_fd(), -1);
}

TEST_F(SocketTest, BindAndListen) {
  Socket server;
  EXPECT_NO_THROW(server.bind(endpoint_));
  EXPECT_NO_THROW(server.listen());
}

TEST_F(SocketTest, AcceptConnection) {
  Socket server;
  server.bind(endpoint_);
  server.listen();

  std::thread client_thread([this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Socket client;
    int ret = ::connect(client.get_fd(), (sockaddr *)&endpoint_.addr_,
                        endpoint_.addr_len_);
    EXPECT_EQ(ret, 0);
  });

  EndPoint client_endpoint;
  int client_fd = server.accept(client_endpoint);
  EXPECT_NE(client_fd, -1);
  close(client_fd);

  client_thread.join();
}

TEST_F(SocketTest, MoveSemantics) {
  Socket socket1;
  int fd1 = socket1.get_fd();

  Socket socket2(std::move(socket1));
  EXPECT_EQ(socket1.get_fd(), -1);
  EXPECT_EQ(socket2.get_fd(), fd1);

  Socket socket3;
  socket3 = std::move(socket2);
  EXPECT_EQ(socket2.get_fd(), -1);
  EXPECT_EQ(socket3.get_fd(), fd1);
}

} // namespace
} // namespace webserver::utils
