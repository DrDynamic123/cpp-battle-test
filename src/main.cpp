#include <Core/Simulation.hpp>
#include <Core/World.hpp>
#include <Features/ScenarioCommandRegistration.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventEmitter.hpp>
#include <IO/System/EventLog.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(const int argc, char** argv)
{
	try
	{
		if (argc != 2)
		{
			throw std::runtime_error("Error: no scenario file specified");
		}

		std::ifstream file(argv[1]);
		if (!file)
		{
			throw std::runtime_error("Error: file not found - " + std::string(argv[1]));
		}

		sw::EventLog eventLog;
		sw::io::EventEmitter events(eventLog);
		sw::World world;
		sw::Simulation simulation(world);

		sw::io::CommandParser parser;
		sw::features::registerScenarioCommands(parser, simulation, events);

		parser.parse(file);
		simulation.run(events);
		return 0;
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
}
