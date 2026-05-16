#pragma once

#include <Core/IUnit.hpp>
#include <Core/Position.hpp>
#include <Core/Types.hpp>

#include <memory>

namespace sw
{
	class Simulation;
}

namespace sw::io
{
	class EventEmitter;
}

namespace sw::features
{
	void createMap(Simulation& simulation, io::EventEmitter& events, int width, int height);

	void spawnUnit(Simulation& simulation, io::EventEmitter& events, std::unique_ptr<IUnit> unit);

	void startMarch(Simulation& simulation, io::EventEmitter& events, UnitId id, Position target);
}
