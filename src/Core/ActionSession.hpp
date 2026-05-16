#pragma once

#include "IUnit.hpp"
#include "Position.hpp"
#include "Stats.hpp"
#include "Types.hpp"

#include <cstdint>

namespace sw
{
	class World;

	// Сессия хода: чтение мира и доступ к World. События — через IO::EventEmitter в Features.
	class ActionSession
	{
	public:
		ActionSession(World& world, int tick);

		[[nodiscard]] Position getPosition(UnitId id) const;
		[[nodiscard]] const StatBlock& getStats(UnitId id) const;

		[[nodiscard]] bool isCellOccupied(Position pos) const;
		[[nodiscard]] bool isInside(Position pos) const;
		[[nodiscard]] IUnit& getUnit(UnitId id) const;

		[[nodiscard]] World& world() noexcept;
		[[nodiscard]] const World& world() const noexcept;
		[[nodiscard]] uint64_t tick() const noexcept;

	private:
		World& world_;
		int tick_{};
	};
}
