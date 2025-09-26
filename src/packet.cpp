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
  std::optional<PacketKind> parsed = std::nullopt;
  
  switch (value) {
    case 0: parsed = PacketKind::Ping;
    case 1: parsed = PacketKind::Setup;
    case 2: parsed = PacketKind::GameAction;
    case 3: parsed = PacketKind::ActionDone;
    case 4: parsed = PacketKind::ActionFail;
    case 5: parsed = PacketKind::Error;
    case 6: parsed = PacketKind::MatchStatus;
    case 7: parsed = PacketKind::Pong;
    default: parsed = std::nullopt;
  }

  return parsed;
}

std::array<uint8_t, 4> kparse_into(const PacketKind kind) {
    uint8_t value = 0xFF; // invalid default

    switch (kind) {
        case PacketKind::Ping:        value = 0x00; break;
        case PacketKind::Setup:       value = 0x01; break;
        case PacketKind::GameAction:  value = 0x02; break;
        case PacketKind::ActionDone:  value = 0x03; break;
        case PacketKind::ActionFail:  value = 0x04; break;
        case PacketKind::Error:       value = 0x05; break;
        case PacketKind::MatchStatus: value = 0x06; break;
        case PacketKind::Pong:        value = 0x07; break;
    }

    return { value, 0x00, 0x00, 0x00 };
}

 
uint32_t parse_32(const std::array<uint8_t, 4> &bytes) {
  return (uint32_t)bytes[0]|
         ((uint32_t)bytes[1] << 8)|
         ((uint32_t)bytes[2] << 16)|
         ((uint32_t)bytes[3] << 24);
}

