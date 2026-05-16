#include "RangedAttackRule.hpp"

#include <Core/ActionSession.hpp>
#include <Features/Common/Actions.hpp>
#include <Features/Common/Navigation.hpp>
#include <Features/Stats.hpp>

#include <stdexcept>

namespace sw::features
{
	RangedAttackRule::RangedAttackRule(
		const std::string_view damageStat,
		const int minDistance,
		const std::string_view maxDistanceStat,
		const bool requireNoAdjacentUnits) :
			damageStat_(damageStat),
			minDistance_(minDistance),
			maxDistanceStat_(maxDistanceStat),
			hasFixedMaxDistance_(false),
			requireNoAdjacentUnits_(requireNoAdjacentUnits)
	{}

	RangedAttackRule::RangedAttackRule(
		const std::string_view damageStat,
		const int minDistance,
		const int maxDistance,
		const bool requireNoAdjacentUnits) :
			damageStat_(damageStat),
			minDistance_(minDistance),
			fixedMaxDistance_(maxDistance),
			hasFixedMaxDistance_(true),
			requireNoAdjacentUnits_(requireNoAdjacentUnits)
	{}

	bool RangedAttackRule::execute(const UnitId self, ActionSession& session, io::EventEmitter& events)
	{
		if (requireNoAdjacentUnits_ && !navigation::unitsInRange(session, self, 1, 1).empty())
		{
			return false;
		}

		const auto& unitStats = session.getStats(self);
		const auto damage = stats::get(unitStats, damageStat_);
		const auto maxDistance = hasFixedMaxDistance_ ? fixedMaxDistance_ : stats::get(unitStats, maxDistanceStat_);
		if (maxDistance < minDistance_)
		{
			throw std::runtime_error("Ranged attack max distance is less than min distance");
		}

		const auto targets = navigation::unitsInRange(session, self, minDistance_, maxDistance);
		if (targets.empty())
		{
			return false;
		}

		actions::dealDamage(session, events, self, targets.front(), damage);
		return true;
	}
}
