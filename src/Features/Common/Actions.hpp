#pragma once

#include <Core/Position.hpp>
#include <Core/Types.hpp>

namespace sw
{
	class ActionSession;
}

namespace sw::io
{
	class EventEmitter;
}

namespace sw::features::actions
{
	void dealDamage(ActionSession& session, io::EventEmitter& events, UnitId attacker, UnitId target, int amount);
	void healUnit(ActionSession& session, io::EventEmitter& events, UnitId healer, UnitId target, int amount);
	void moveUnit(ActionSession& session, io::EventEmitter& events, UnitId id, Position to);
	void endMarch(ActionSession& session, io::EventEmitter& events, UnitId id);
	void scheduleRemoval(ActionSession& session, io::EventEmitter& events, UnitId id);
}
