#pragma once

#include "EventLog.hpp"

#include <cstdint>
#include <utility>

namespace sw::io
{
	// Тонкая обёртка над EventLog: новое событие = struct в IO/Events + emit.log(tick, event).
	class EventEmitter
	{
	public:
		explicit EventEmitter(EventLog& log) noexcept :
				log_(log)
		{}

		template <class TEvent>
		void log(const uint64_t tick, TEvent&& event)
		{
			log_.log(tick, std::forward<TEvent>(event));
		}

	private:
		EventLog& log_;
	};
}
