#include "client.h"
#include "utils/stream_logger.h"

namespace webserver::client {

Client::Client() : socket_(std::make_unique<util::Socket>()) {
  util::errif(socket_->get_fd() == -1, "failed to create socket");
}

Client::~Client() {
  if (socket_->get_fd() >= 0) {
    close(socket_->get_fd());
  }
}

Client::Client(Client &&other) noexcept : socket_(std::move(other.socket_)) {
  other.socket_ = nullptr;
}

Client &Client::operator=(Client &&other) noexcept {
  socket_ = std::move(other.socket_);
  return *this;
}

void Client::connect(const util::EndPoint &endpoint) {
  util::errif(socket_->connect(endpoint) == -1, "failed to connect to server");
}

void Client::send(std::string msg) {
  ssize_t bytes = write(socket_->get_fd(), msg.c_str(), msg.size());
  if (bytes == -1) {
    LOG() << "[Client] failed to send message to server, errno: " << errno;
  }
}

std::string Client::recv() {
  std::string result;
  char buffer[4096];

  while (true) {
    ssize_t bytes = read(socket_->get_fd(), buffer, sizeof(buffer));
    if (bytes == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue;
      }
      LOG() << "[Client] failed to receive message from server, errno: "
            << errno;
      return std::string();
    }

    if (bytes == 0) {
      LOG() << "[Client] connection closed by server";
      break;
    }

    result.append(buffer, bytes);

    if (bytes < static_cast<ssize_t>(sizeof(buffer) - 1)) {
      result.append("\0");
      LOG() << "[Client] received finish from server";
      break;
    }
  }

  return result;
}
} // namespace webserver::client
