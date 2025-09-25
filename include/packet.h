#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <optional>
#include <vector>

enum PacketKind {
   Ping = 0,
   Setup = 1,
   GameAction = 2,
   ActionDone = 3,
   ActionFail = 4,
   Error = 5,
   MatchStatus = 6,
   Pong = 7,
};

struct Packet {
  int id;
  int size;
  PacketKind kind;
  std::vector<uint8_t> body;
  
  static std::optional<Packet> parse(std::vector<uint8_t> bytes);
  static Packet create(int id, PacketKind kind, std::vector<uint8_t> body);
};

enum Setup {
  Connection = 1,
  Reconnection = 2,
  Initialization = 3,
  Ready = 4
};

std::vector<uint8_t> kparse_into(PacketKind &kind);
PacketKind kparse_from(std::vector<uint8_t> &bytes);

std::vector<uint8_t> sparse_into(enum::Setup &setup);
enum::Setup sparse_from(std::vector<uint8_t> &bytes);

#endif
