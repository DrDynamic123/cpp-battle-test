#pragma once

#include "Position.hpp"
#include "Types.hpp"
#include "IUnit.hpp"
#include "World.hpp"

#include <cstdint>
#include <memory>

namespace sw::io
{
	class EventEmitter;
}

namespace sw
{
	class Simulation
	{
	public:
		explicit Simulation(World& world);

		void createMap(int width, int height);
		void spawnUnit(std::unique_ptr<IUnit> unit);
		void run(io::EventEmitter& events);

		[[nodiscard]] World& world() noexcept;
		[[nodiscard]] const World& world() const noexcept;
		[[nodiscard]] uint64_t eventTick() const noexcept;

	private:
		void tick(io::EventEmitter& events);
		void removeDeadUnits();
		[[nodiscard]] bool shouldStop() const noexcept;

		World& world_;
		int currentTick_{1};
		int activeLastTick_{-1};
		int nextSpawnOrder_{0};
	};
}
