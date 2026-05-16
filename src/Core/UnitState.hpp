#pragma once

#include "Position.hpp"
#include "Types.hpp"

namespace sw
{
	struct UnitState
	{
		UnitId id{};
		Position pos{};
		int spawnOrder{};
		bool pendingRemoval = false;
	};
}
