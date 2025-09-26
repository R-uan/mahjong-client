#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <shared_mutex>
#include <vector>

struct TcpClient {
  int socketFd;
  std::shared_mutex socketMtx;
  std::atomic<bool> listening;

  TcpClient(int fd) : socketFd(fd), listening(std::atomic<bool>(false)), socketMtx(std::shared_mutex()) {}

  static std::shared_ptr<TcpClient> connect(int port);

  void listen();
  void handle_incoming(std::vector<uint8_t> bytes);
};

#endif
