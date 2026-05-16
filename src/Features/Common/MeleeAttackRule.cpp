#include "MeleeAttackRule.hpp"

#include <Core/ActionSession.hpp>
#include <Features/Common/Actions.hpp>
#include <Features/Common/Navigation.hpp>
#include <Features/Stats.hpp>

namespace sw::features
{
	MeleeAttackRule::MeleeAttackRule(const std::string_view damageStat) :
			damageStat_(damageStat)
	{}

	bool MeleeAttackRule::execute(const UnitId self, ActionSession& session, io::EventEmitter& events)
	{
		const auto targets = navigation::unitsInRange(session, self, 1, 1);
		if (targets.empty())
		{
			return false;
		}

		const auto damage = stats::get(session.getStats(self), damageStat_);
		actions::dealDamage(session, events, self, targets.front(), damage);
		return true;
	}
}
