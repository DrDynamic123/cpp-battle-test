#include "HunterFactory.hpp"

#include "Hunter.hpp"

namespace sw::features::hunter
{
	std::unique_ptr<IUnit> make(
		const UnitId id,
		const Position pos,
		const int hp,
		const int agility,
		const int strength,
		const int range)
	{
		return std::make_unique<Hunter>(id, pos, hp, agility, strength, range);
	}
}
