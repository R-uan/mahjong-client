#include "packet.h"
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

uint32_t parse_32(const std::array<uint8_t, 4> &bytes); 

std::optional<Packet> Packet::parse(std::vector<uint8_t> bytes) {
  if (bytes.size() < 12) return std::nullopt;
  uint32_t id = parse_32({bytes[0], bytes[1], bytes[2], bytes[3]});
  std::optional<PacketKind> packetKind = kparse_from({bytes[4], bytes[5], bytes[6], bytes[7]});
  if (packetKind == std::nullopt) return std::nullopt;
  uint32_t size = parse_32({bytes[8], bytes[9], bytes[10], bytes[11]});
  std::vector<uint8_t> body(bytes.begin() + 12, bytes.end());
  return Packet(id, packetKind.value(), size, body);
}

Packet Packet::create(int id, PacketKind kind, std::vector<uint8_t> body) {
  // `size` is not included in the size calculation.
  int size = (8 + 2 + body.size());
  return Packet(id, kind, size, body);
}

std::optional<PacketKind> kparse_from(const std::array<uint8_t, 4> &bytes) {
  uint32_t value = parse_32(bytes);
    
  switch (value) {
    case 0: return PacketKind::Ping;
    case 1: return PacketKind::Setup;
    case 2: return PacketKind::GameAction;
    case 3: return PacketKind::ActionDone;
    case 4: return PacketKind::ActionFail;
    case 5: return PacketKind::Error;
    case 6: return PacketKind::MatchStatus;
    case 7: return PacketKind::Pong;
    default: return std::nullopt;
  }
}

std::array<uint8_t, 4> kparse_into(PacketKind &kind) {
  switch (kind) {
    case PacketKind::Ping: return { 0x00, 0x00, 0x00, 0x00 };
    case PacketKind::Setup: return { 0x01, 0x00, 0x00, 0x00 };
    case PacketKind::GameAction: return { 0x02, 0x00, 0x00, 0x00 };
    case PacketKind::ActionDone: return { 0x03, 0x00, 0x00, 0x00 };
    case PacketKind::ActionFail: return { 0x04, 0x00, 0x00, 0x00 };
    case PacketKind::Error: return { 0x05, 0x00, 0x00, 0x00 };
    case PacketKind::MatchStatus: return { 0x06, 0x00, 0x00, 0x00 };
    case PacketKind::Pong: return { 0x07, 0x00, 0x00, 0x00 };
  }
}
 
uint32_t parse_32(const std::array<uint8_t, 4> &bytes) {
  return (uint32_t)bytes[0]|
         ((uint32_t)bytes[1] << 8)|
         ((uint32_t)bytes[2] << 16)|
         ((uint32_t)bytes[3] << 24);
}

