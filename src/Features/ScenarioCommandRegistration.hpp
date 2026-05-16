#pragma once

namespace sw
{
	class Simulation;
}

namespace sw::io
{
	class CommandParser;
	class EventEmitter;
}

namespace sw::features
{
	void registerScenarioCommands(io::CommandParser& parser, Simulation& simulation, io::EventEmitter& events);
}
