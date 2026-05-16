#include "SimulationCommands.hpp"

#include <Core/Simulation.hpp>
#include <Features/Capabilities/IMarchable.hpp>
#include <IO/SimulationEvents.hpp>

#include <stdexcept>
#include <string>

namespace sw::features
{
	void createMap(Simulation& simulation, io::EventEmitter& events, const int width, const int height)
	{
		simulation.createMap(width, height);
		io::events::mapCreated(events, simulation.eventTick(), static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	}

	void spawnUnit(Simulation& simulation, io::EventEmitter& events, std::unique_ptr<IUnit> unit)
	{
		const auto unitId = unit->id();
		const auto type = unit->type();
		const auto pos = unit->position();
		simulation.spawnUnit(std::move(unit));
		io::events::unitSpawned(events, simulation.eventTick(), unitId, type, pos);
	}

	void startMarch(Simulation& simulation, io::EventEmitter& events, const UnitId id, const Position target)
	{
		auto& world = simulation.world();
		auto& unit = world.getUnit(id);
		auto* marchable = dynamic_cast<IMarchable*>(&unit);
		if (marchable == nullptr)
		{
			throw std::runtime_error("Unit is not marchable: " + std::to_string(id.value));
		}
		if (!world.isInside(target))
		{
			throw std::runtime_error("March target is outside the map");
		}

		const auto from = unit.position();
		marchable->setMarchTarget(target);
		io::events::marchStarted(events, simulation.eventTick(), id, from, target);
	}
}
