#pragma once

#include "IActionRule.hpp"

#include <string_view>

namespace sw::features
{
	class MeleeAttackRule final : public IActionRule
	{
	public:
		explicit MeleeAttackRule(std::string_view damageStat);

		bool execute(UnitId self, ActionSession& session, io::EventEmitter& events) override;

	private:
		std::string_view damageStat_;
	};
}
