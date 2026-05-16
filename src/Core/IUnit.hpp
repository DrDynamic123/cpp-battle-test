#pragma once

#include "Position.hpp"
#include "Stats.hpp"
#include "Types.hpp"
#include "UnitState.hpp"

#include <cstdint>
#include <string_view>

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
	class IUnit
	{
	public:
		virtual ~IUnit() = default;

		IUnit() = default;
		IUnit(const IUnit&) = delete;
		IUnit& operator=(const IUnit&) = delete;
		IUnit(IUnit&&) = delete;
		IUnit& operator=(IUnit&&) = delete;

		virtual UnitId id() const = 0;
		virtual Position position() const = 0;
		virtual void setPosition(Position pos) = 0;

		virtual int spawnOrder() const = 0;
		virtual void setSpawnOrder(int order) = 0;

		virtual std::string_view type() const = 0;
		virtual const StatBlock& stats() const = 0;

		virtual bool pendingRemoval() const = 0;
		virtual void markPendingRemoval() = 0;
		virtual bool occupiesCell() const
		{
			return true;
		}

		virtual bool act(ActionSession& session, io::EventEmitter& events) = 0;
	};
}
