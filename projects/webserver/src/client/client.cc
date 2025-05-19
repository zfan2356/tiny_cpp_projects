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

void Client::send(std::string &&msg) { socket_->send(msg); }

} // namespace webserver::client
