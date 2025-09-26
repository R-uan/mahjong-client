#include "tcp_client.h"
#include "packet.h"
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <pthread.h>
#include <shared_mutex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

std::shared_ptr<TcpClient> TcpClient::connect(int port) {
  const int socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    exit(5);
  }

  sockaddr_in serverAddr {};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (::connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
      std::cerr << "Failed to connect to address" << std::endl;
      exit(6);
  }
  std::cout << "Connected on: 127.0.0.1:" << port << std::endl;
  return std::make_shared<TcpClient>(socketFd);
}

void TcpClient::listen() {
  char buffer[4086];
  std::cout << "Listening" << std::endl;
  this->listening.store(true, std::memory_order_release);
  while (this->listening.load(std::memory_order_acquire)) {
    std::shared_lock<std::shared_mutex> socketGuard(this->socketMtx);
    int n = read(this->socketFd, buffer, sizeof(buffer));
    socketGuard.unlock();
    
    if (n <= 0) break;
    const std::vector<uint8_t> bytes(buffer, buffer + n);
    this->handle_incoming(bytes);
  }

  close(this->socketFd);
}

void TcpClient::handle_incoming(std::vector<uint8_t> bytes) {
  auto packet = Packet::parse(bytes);
  if (!packet.has_value()) return;
}
