#include "BehaviorSequence.hpp"

namespace sw::features
{
	void BehaviorSequence::addRule(std::unique_ptr<IActionRule> rule)
	{
		rules_.push_back(std::move(rule));
	}

	bool BehaviorSequence::act(const UnitId self, ActionSession& session, io::EventEmitter& events)
	{
		for (const auto& rule : rules_)
		{
			if (rule->execute(self, session, events))
			{
				return true;
			}
		}
		return false;
	}
}
