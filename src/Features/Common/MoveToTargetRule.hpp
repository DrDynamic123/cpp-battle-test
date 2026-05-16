#pragma once

#include "IActionRule.hpp"

namespace sw::features
{
	class MoveToTargetRule final : public IActionRule
	{
	public:
		bool execute(UnitId self, ActionSession& session, io::EventEmitter& events) override;
	};
}
