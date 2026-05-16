#pragma once

#include <Core/IUnit.hpp>
#include <Core/Position.hpp>
#include <Core/Types.hpp>

#include <memory>

namespace sw::features::hunter
{
	[[nodiscard]] std::unique_ptr<IUnit> make(UnitId id, Position pos, int hp, int agility, int strength, int range);
}
