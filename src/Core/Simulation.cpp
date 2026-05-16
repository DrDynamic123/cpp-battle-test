#include "Simulation.hpp"

#include "ActionSession.hpp"

#include <IO/System/EventEmitter.hpp>

#include <stdexcept>

namespace sw
{
	Simulation::Simulation(World& world) :
			world_(world)
	{}

	void Simulation::createMap(const int width, const int height)
	{
		world_.createMap(width, height);
	}

	void Simulation::spawnUnit(std::unique_ptr<IUnit> unit)
	{
		if (!unit)
		{
			throw std::runtime_error("Cannot spawn null unit");
		}
		unit->setSpawnOrder(nextSpawnOrder_++);
		world_.spawnUnit(std::move(unit));
	}

	void Simulation::run(io::EventEmitter& events)
	{
		while (true)
		{
			++currentTick_;
			tick(events);
			if (shouldStop())
			{
				return;
			}
		}
	}

	void Simulation::tick(io::EventEmitter& events)
	{
		removeDeadUnits();
		if (shouldStop())
		{
			return;
		}

		const auto order = world_.unitOrder();
		ActionSession session(world_, currentTick_);
		int activeCount = 0;

		for (const auto id : order)
		{
			if (!world_.contains(id))
			{
				continue;
			}

			auto& unit = world_.getUnit(id);
			if (unit.act(session, events))
			{
				++activeCount;
			}
		}

		activeLastTick_ = activeCount;
	}

	void Simulation::removeDeadUnits()
	{
		const auto order = world_.unitOrder();
		for (const auto id : order)
		{
			if (!world_.contains(id))
			{
				continue;
			}
			if (world_.getUnit(id).pendingRemoval())
			{
				world_.removeUnit(id);
			}
		}
	}

	bool Simulation::shouldStop() const noexcept
	{
		return world_.unitCount() <= 1 || activeLastTick_ == 0;
	}

	World& Simulation::world() noexcept
	{
		return world_;
	}

	const World& Simulation::world() const noexcept
	{
		return world_;
	}

	uint64_t Simulation::eventTick() const noexcept
	{
		return static_cast<uint64_t>(currentTick_);
	}
}
