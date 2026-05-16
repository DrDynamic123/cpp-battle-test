#pragma once

#include "IActionRule.hpp"

#include <Core/IBehavior.hpp>

#include <memory>
#include <vector>

namespace sw::features
{
	class BehaviorSequence final : public IBehavior
	{
	public:
		void addRule(std::unique_ptr<IActionRule> rule);

		bool act(UnitId self, ActionSession& session, io::EventEmitter& events) override;

	private:
		std::vector<std::unique_ptr<IActionRule>> rules_;
	};
}
