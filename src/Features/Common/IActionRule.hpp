#pragma once

#include <Core/Types.hpp>

namespace sw
{
	class ActionSession;
}

namespace sw::io
{
	class EventEmitter;
}

namespace sw::features
{
	class IActionRule
	{
	public:
		virtual ~IActionRule() = default;

		IActionRule() = default;
		IActionRule(const IActionRule&) = delete;
		IActionRule& operator=(const IActionRule&) = delete;
		IActionRule(IActionRule&&) = delete;
		IActionRule& operator=(IActionRule&&) = delete;

		virtual bool execute(UnitId self, ActionSession& session, io::EventEmitter& events) = 0;
	};
}
