#include "SimulationEvents.hpp"

#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>

#include <stdexcept>
#include <string>

namespace sw::io::events
{
	namespace
	{
		uint32_t toEventCoordinate(const int value)
		{
			if (value < 0)
			{
				throw std::runtime_error("Cannot log negative coordinate");
			}
			return static_cast<uint32_t>(value);
		}
	}

	void mapCreated(EventEmitter& emit, const uint64_t tick, const uint32_t width, const uint32_t height)
	{
		emit.log(tick, MapCreated{width, height});
	}

	void unitSpawned(
		EventEmitter& emit,
		const uint64_t tick,
		const UnitId unitId,
		const std::string_view unitType,
		const Position pos)
	{
		emit.log(
			tick,
			UnitSpawned{
				unitId.value,
				std::string(unitType),
				toEventCoordinate(pos.x),
				toEventCoordinate(pos.y),
			});
	}

	void marchStarted(
		EventEmitter& emit,
		const uint64_t tick,
		const UnitId unitId,
		const Position from,
		const Position target)
	{
		emit.log(
			tick,
			MarchStarted{
				unitId.value,
				toEventCoordinate(from.x),
				toEventCoordinate(from.y),
				toEventCoordinate(target.x),
				toEventCoordinate(target.y),
			});
	}

	void marchEnded(EventEmitter& emit, const uint64_t tick, const UnitId unitId, const Position pos)
	{
		emit.log(tick, MarchEnded{unitId.value, toEventCoordinate(pos.x), toEventCoordinate(pos.y)});
	}

	void unitMoved(EventEmitter& emit, const uint64_t tick, const UnitId unitId, const Position pos)
	{
		emit.log(tick, UnitMoved{unitId.value, toEventCoordinate(pos.x), toEventCoordinate(pos.y)});
	}

	void unitAttacked(
		EventEmitter& emit,
		const uint64_t tick,
		const UnitId attacker,
		const UnitId target,
		const uint32_t damage,
		const uint32_t targetHp)
	{
		emit.log(tick, UnitAttacked{attacker.value, target.value, damage, targetHp});
	}

	void unitDied(EventEmitter& emit, const uint64_t tick, const UnitId unitId)
	{
		emit.log(tick, UnitDied{unitId.value});
	}
}
