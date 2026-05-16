#pragma once

#include "IActionRule.hpp"

#include <string_view>

namespace sw::features
{
	class RangedAttackRule final : public IActionRule
	{
	public:
		RangedAttackRule(std::string_view damageStat, int minDistance, std::string_view maxDistanceStat, bool requireNoAdjacentUnits);
		RangedAttackRule(std::string_view damageStat, int minDistance, int maxDistance, bool requireNoAdjacentUnits);

		bool execute(UnitId self, ActionSession& session, io::EventEmitter& events) override;

	private:
		std::string_view damageStat_;
		int minDistance_{};
		int fixedMaxDistance_{};
		std::string_view maxDistanceStat_;
		bool hasFixedMaxDistance_{};
		bool requireNoAdjacentUnits_{};
	};
}
