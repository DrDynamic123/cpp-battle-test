#include "ScenarioCommandRegistration.hpp"

#include <Core/Position.hpp>
#include <Core/Simulation.hpp>
#include <Core/Types.hpp>
#include <Features/Hunter/HunterFactory.hpp>
#include <Features/SimulationCommands.hpp>
#include <Features/Swordsman/SwordsmanFactory.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/System/CommandParser.hpp>

#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace sw::features
{
	namespace
	{
		int toPositiveInt(const uint32_t value, const std::string_view fieldName)
		{
			if (value == 0 || value > static_cast<uint32_t>(std::numeric_limits<int>::max()))
			{
				throw std::runtime_error(std::string(fieldName) + " must be a positive int");
			}
			return static_cast<int>(value);
		}

		Position toPosition(const uint32_t x, const uint32_t y)
		{
			if (x > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
				y > static_cast<uint32_t>(std::numeric_limits<int>::max()))
			{
				throw std::runtime_error("Position coordinate is too large");
			}
			return Position{static_cast<int>(x), static_cast<int>(y)};
		}
	}

	void registerScenarioCommands(
		io::CommandParser& parser,
		Simulation& simulation,
		io::EventEmitter& events)
	{
		parser.add<io::CreateMap>(
			[&simulation, &events](const io::CreateMap& command)
			{
				createMap(simulation, events, command.width, command.height);
			});

		parser.add<io::SpawnSwordsman>(
			[&simulation, &events](const io::SpawnSwordsman& command)
			{
				const auto hp = toPositiveInt(command.hp, "hp");
				const auto strength = toPositiveInt(command.strength, "strength");
				spawnUnit(
					simulation,
					events,
					swordsman::make(
						UnitId{command.unitId},
						toPosition(command.x, command.y),
						hp,
						strength));
			});

		parser.add<io::SpawnHunter>(
			[&simulation, &events](const io::SpawnHunter& command)
			{
				const auto hp = toPositiveInt(command.hp, "hp");
				const auto agility = toPositiveInt(command.agility, "agility");
				const auto strength = toPositiveInt(command.strength, "strength");
				const auto range = toPositiveInt(command.range, "range");
				if (range < 2)
				{
					throw std::runtime_error("range must be at least 2");
				}

				spawnUnit(
					simulation,
					events,
					hunter::make(
						UnitId{command.unitId},
						toPosition(command.x, command.y),
						hp,
						agility,
						strength,
						range));
			});

		parser.add<io::March>(
			[&simulation, &events](const io::March& command)
			{
				startMarch(simulation, events, UnitId{command.unitId}, toPosition(command.targetX, command.targetY));
			});
	}
}
