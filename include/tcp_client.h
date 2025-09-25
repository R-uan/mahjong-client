#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "protocol.h"
#include <atomic>
#include <cstdint>
#include <vector>

struct TcpClient {
  int socketFd;
  Protocol protocol;
  std::atomic<bool> listening {false};

  TcpClient(int fd) : socketFd(fd) {}

  static TcpClient connect(int port);

  void listen();
  void handle_incoming(std::vector<uint8_t> bytes);
};

#endif
