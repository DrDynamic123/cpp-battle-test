#include "MoveToTargetRule.hpp"

#include <Core/ActionSession.hpp>
#include <Core/Position.hpp>
#include <Features/Capabilities/IMarchable.hpp>
#include <Features/Common/Actions.hpp>
#include <Features/Common/Navigation.hpp>

namespace sw::features
{
	bool MoveToTargetRule::execute(const UnitId self, ActionSession& session, io::EventEmitter& events)
	{
		auto* marchable = dynamic_cast<IMarchable*>(&session.getUnit(self));
		if (marchable == nullptr)
		{
			return false;
		}

		const auto marchTarget = marchable->getMarchTarget();
		if (!marchTarget.has_value())
		{
			return false;
		}

		const auto position = session.getPosition(self);
		const auto target = marchTarget.value();
		const auto distance = navigation::chebyshevDistance(position, target);
		if (distance == 0)
		{
			actions::endMarch(session, events, self);
			return false;
		}

		if (distance == 1 && session.isCellOccupied(target))
		{
			actions::endMarch(session, events, self);
			return false;
		}

		const auto next = navigation::bestStepToward(session, self, target);
		if (!next.has_value())
		{
			return false;
		}

		const auto nextPosition = next.value();
		actions::moveUnit(session, events, self, nextPosition);
		if (nextPosition == target)
		{
			actions::endMarch(session, events, self);
		}
		return true;
	}
}
