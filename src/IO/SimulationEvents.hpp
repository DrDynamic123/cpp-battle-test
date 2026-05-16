#pragma once

#include <Core/Position.hpp>
#include <Core/Types.hpp>
#include <IO/System/EventEmitter.hpp>

#include <cstdint>
#include <string_view>

namespace sw::io::events
{
	void mapCreated(EventEmitter& emit, uint64_t tick, uint32_t width, uint32_t height);

	void unitSpawned(
		EventEmitter& emit,
		uint64_t tick,
		UnitId unitId,
		std::string_view unitType,
		Position pos);

	void marchStarted(
		EventEmitter& emit,
		uint64_t tick,
		UnitId unitId,
		Position from,
		Position target);

	void marchEnded(EventEmitter& emit, uint64_t tick, UnitId unitId, Position pos);

	void unitMoved(EventEmitter& emit, uint64_t tick, UnitId unitId, Position pos);

	void unitAttacked(
		EventEmitter& emit,
		uint64_t tick,
		UnitId attacker,
		UnitId target,
		uint32_t damage,
		uint32_t targetHp);

	void unitDied(EventEmitter& emit, uint64_t tick, UnitId unitId);
}
