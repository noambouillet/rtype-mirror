/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Protocol function and structs definition
*/

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace rtype::network {

static constexpr std::size_t HEADER_SIZE = 16;
static constexpr std::size_t MAX_DATAGRAM_SIZE = 1200;
static constexpr std::array<std::uint8_t, 4> MAGIC{'R', 'T', 'Y', 'P'};
static constexpr std::uint8_t VERSION = 1;
static constexpr std::uint32_t HALF_SEQUENCE = 2147483648;
static constexpr std::uint8_t BUTTON_UP = 0x01;
static constexpr std::uint8_t BUTTON_DOWN = 0x02;
static constexpr std::uint8_t BUTTON_LEFT = 0x04;
static constexpr std::uint8_t BUTTON_RIGHT = 0x08;
static constexpr std::uint8_t BUTTON_FIRE = 0x10;
static constexpr std::uint8_t MAX_BUTTON_STATE = 0b00011111;
static constexpr std::size_t MAX_SNAPSHOT_ENTITIES = 64;
static constexpr std::uint8_t NO_OWNER = 255;

enum class PacketType : std::uint8_t {
	JoinRequest = 0x01,
	JoinAccepted = 0x02,
	JoinRejected = 0x03,
	Disconnect = 0x04,
	PlayerLeft = 0x05,
	PlayerInput = 0x10,
	WorldSnapshot = 0x20,
	EntitySpawn = 0x21,
	EntityDestroy = 0x22,
	EntityDamage = 0x23,
	PlayerDeath = 0x24,
	Error = 0x67,
};

enum class ErrorCode : std::uint8_t {
	UnsupportedVersion = 1,
	InvalidLength = 2,
	UnknownType = 3,
	WrongDirection = 4,
	InvalidSession = 5,
	InvalidField = 6,
};

/// Entity kinds carried in snapshot records (RFC "Entity record").
enum class EntityType : std::uint8_t {
	Player = 1,
	Enemy = 2,
	PlayerProjectile = 3,
	EnemyProjectile = 4,
};

/// Enemy kinds, carried as the variant of an Enemy record.
enum class EnemyKind : std::uint8_t {
	Patapata = 0,
	Bug = 1,
	Scant = 2,
	Bink = 3,
};

/// Enemy shot kinds, carried as the variant of an EnemyProjectile record.
enum class ShotKind : std::uint8_t {
	Aimed = 0,
	Beam = 1,
};

struct Header {
	PacketType type;
	std::uint16_t payloadSize;
	std::uint32_t sequence;
	std::uint32_t sessionId;
};

struct ParsedHeader {
	std::optional<Header> header;
	std::optional<ErrorCode> error;
	std::uint8_t offendingType;
	std::uint32_t offendingSequence;
};

ParsedHeader parseClientHeader(const std::vector<std::uint8_t>& bytes);
std::optional<Header> parseServerHeader(const std::vector<std::uint8_t>& bytes);
std::optional<ErrorCode> validateClientPayload(const Header& header, const std::vector<std::uint8_t>& bytes);
std::vector<std::uint8_t> makePacket(PacketType type, std::uint32_t sequence,
	std::uint32_t sessionId, const std::vector<std::uint8_t>& payload);
std::vector<std::uint8_t> makeError(ErrorCode code, const ParsedHeader& rejected);
bool isNewerSequence(std::uint32_t candidate, std::uint32_t previous);
void appendU16(std::vector<std::uint8_t>& bytes, std::uint16_t value);
void appendU32(std::vector<std::uint8_t>& bytes, std::uint32_t value);
void appendU64(std::vector<std::uint8_t>& bytes, std::uint64_t value);
std::uint32_t readU32(const std::vector<std::uint8_t>& bytes, std::size_t offset);
std::uint64_t readU64(const std::vector<std::uint8_t>& bytes, std::size_t offset);

}
