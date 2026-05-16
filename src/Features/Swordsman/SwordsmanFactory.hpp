#pragma once

#include <Core/IUnit.hpp>
#include <Core/Position.hpp>
#include <Core/Types.hpp>

#include <memory>

namespace sw::features::swordsman
{
	[[nodiscard]] std::unique_ptr<IUnit> make(UnitId id, Position pos, int hp, int strength);
}
