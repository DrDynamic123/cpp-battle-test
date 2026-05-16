#include "SwordsmanFactory.hpp"

#include "Swordsman.hpp"

namespace sw::features::swordsman
{
	std::unique_ptr<IUnit> make(const UnitId id, const Position pos, const int hp, const int strength)
	{
		return std::make_unique<Swordsman>(id, pos, hp, strength);
	}
}
