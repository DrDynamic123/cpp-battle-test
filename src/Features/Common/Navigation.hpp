#pragma once

#include <Core/Position.hpp>
#include <Core/Types.hpp>

#include <optional>
#include <vector>

namespace sw
{
	class ActionSession;
}

namespace sw::features::navigation
{
	[[nodiscard]] int chebyshevDistance(Position lhs, Position rhs) noexcept;
	[[nodiscard]] bool isAdjacent(Position lhs, Position rhs) noexcept;

	[[nodiscard]] std::vector<UnitId> unitsInRange(
		const ActionSession& session,
		UnitId self,
		int minDistance,
		int maxDistance);

	// Предпочтительный шаг (x+sign(dx), y+sign(dy)); иначе соседняя клетка с минимальным Chebyshev до цели.
	[[nodiscard]] std::optional<Position> bestStepToward(
		const ActionSession& session,
		UnitId id,
		Position target);
}
