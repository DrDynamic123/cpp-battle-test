#include "ActionSession.hpp"

#include "World.hpp"

namespace sw
{
	ActionSession::ActionSession(World& world, const int tick) :
			world_(world),
			tick_(tick)
	{}

	Position ActionSession::getPosition(const UnitId id) const
	{
		return world_.getUnit(id).position();
	}

	const StatBlock& ActionSession::getStats(const UnitId id) const
	{
		return world_.getUnit(id).stats();
	}

	bool ActionSession::isCellOccupied(const Position pos) const
	{
		return world_.isCellOccupied(pos);
	}

	bool ActionSession::isInside(const Position pos) const
	{
		return world_.isInside(pos);
	}

	IUnit& ActionSession::getUnit(const UnitId id) const
	{
		return world_.getUnit(id);
	}

	World& ActionSession::world() noexcept
	{
		return world_;
	}

	const World& ActionSession::world() const noexcept
	{
		return world_;
	}

	uint64_t ActionSession::tick() const noexcept
	{
		return static_cast<uint64_t>(tick_);
	}
}
