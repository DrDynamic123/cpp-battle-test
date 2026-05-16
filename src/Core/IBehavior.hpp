#pragma once

#include "Types.hpp"

namespace sw
{
	class ActionSession;
}

namespace sw::io
{
	class EventEmitter;
}

namespace sw
{
	class IBehavior
	{
	public:
		virtual ~IBehavior() = default;

		IBehavior() = default;
		IBehavior(const IBehavior&) = delete;
		IBehavior& operator=(const IBehavior&) = delete;
		IBehavior(IBehavior&&) = delete;
		IBehavior& operator=(IBehavior&&) = delete;

		virtual bool act(UnitId self, ActionSession& session, io::EventEmitter& events) = 0;
	};
}
