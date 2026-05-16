#include "Navigation.hpp"

#include <Core/ActionSession.hpp>
#include <Core/Position.hpp>
#include <Core/World.hpp>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <stdexcept>

namespace sw::features::navigation
{
	namespace
	{
		int sign(const int value) noexcept
		{
			return (value > 0) - (value < 0);
		}
	}

	int chebyshevDistance(const Position lhs, const Position rhs) noexcept
	{
		return std::max(std::abs(lhs.x - rhs.x), std::abs(lhs.y - rhs.y));
	}

	bool isAdjacent(const Position lhs, const Position rhs) noexcept
	{
		return chebyshevDistance(lhs, rhs) == 1;
	}

	std::vector<UnitId> unitsInRange(
		const ActionSession& session,
		const UnitId self,
		const int minDistance,
		const int maxDistance)
	{
		if (minDistance < 0 || maxDistance < minDistance)
		{
			throw std::runtime_error("Invalid distance range");
		}

		const auto from = session.getPosition(self);
		const auto& world = session.world();
		auto ids = world.unitOrder();
		ids.erase(
			std::remove_if(
				ids.begin(),
				ids.end(),
				[&world, from, self, minDistance, maxDistance](const UnitId id)
				{
					if (id == self)
					{
						return true;
					}

					const auto& unit = world.getUnit(id);
					if (unit.pendingRemoval())
					{
						return true;
					}

					const auto distance = chebyshevDistance(from, unit.position());
					return distance < minDistance || distance > maxDistance;
				}),
			ids.end());
		return ids;
	}

	std::optional<Position> bestStepToward(
		const ActionSession& session,
		const UnitId id,
		const Position target)
	{
		const auto& world = session.world();
		const auto& unit = world.getUnit(id);
		const auto currentDistance = chebyshevDistance(unit.position(), target);
		if (currentDistance == 0)
		{
			return std::nullopt;
		}

		const auto unitPosition = unit.position();
		const Position directStep{sign(target.x - unitPosition.x), sign(target.y - unitPosition.y)};
		const std::array<Position, 9> directions{
			directStep,
			Position{-1, -1},
			Position{0, -1},
			Position{1, -1},
			Position{-1, 0},
			Position{1, 0},
			Position{-1, 1},
			Position{0, 1},
			Position{1, 1},
		};

		std::optional<Position> best;
		auto bestDistance = currentDistance;
		const auto requiresFreeCell = unit.occupiesCell();

		for (const auto direction : directions)
		{
			if (direction.x == 0 && direction.y == 0)
			{
				continue;
			}

			const Position candidate{unitPosition.x + direction.x, unitPosition.y + direction.y};
			if (!world.isInside(candidate))
			{
				continue;
			}
			if (requiresFreeCell && world.isCellOccupied(candidate))
			{
				continue;
			}

			const auto distance = chebyshevDistance(candidate, target);
			if (distance < bestDistance)
			{
				best = candidate;
				bestDistance = distance;
			}
		}

		return best;
	}
}
