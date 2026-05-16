cmake_minimum_required(VERSION 3.27)
project(sw_battle_test LANGUAGES CXX)

add_executable(sw_battle_test)

target_sources(sw_battle_test
	PRIVATE
		src/main.cpp
		src/Core/ActionSession.cpp
		src/Core/ActionSession.hpp
		src/Core/IBehavior.hpp
		src/Core/IUnit.hpp
		src/Core/Position.hpp
		src/Core/Simulation.cpp
		src/Core/Simulation.hpp
		src/Core/Stats.hpp
		src/Core/Types.hpp
		src/Core/UnitState.hpp
		src/Core/World.cpp
		src/Core/World.hpp
		src/Features/Common/Actions.cpp
		src/Features/Common/Actions.hpp
		src/Features/Common/BehaviorSequence.cpp
		src/Features/Common/BehaviorSequence.hpp
		src/Features/Common/IActionRule.hpp
		src/Features/Common/Navigation.cpp
		src/Features/Common/Navigation.hpp
		src/Features/Common/MeleeAttackRule.cpp
		src/Features/Common/MeleeAttackRule.hpp
		src/Features/Common/MoveToTargetRule.cpp
		src/Features/Common/MoveToTargetRule.hpp
		src/Features/Common/RangedAttackRule.cpp
		src/Features/Common/RangedAttackRule.hpp
		src/Features/Capabilities/IDamageable.hpp
		src/Features/Capabilities/IMarchable.hpp
		src/Features/Capabilities/IMovable.hpp
		src/Features/Stats.hpp
		src/Features/Hunter/Hunter.cpp
		src/Features/Hunter/Hunter.hpp
		src/Features/Hunter/HunterFactory.cpp
		src/Features/Hunter/HunterFactory.hpp
		src/Features/Swordsman/Swordsman.cpp
		src/Features/Swordsman/Swordsman.hpp
		src/Features/Swordsman/SwordsmanFactory.cpp
		src/Features/Swordsman/SwordsmanFactory.hpp
		src/Features/ScenarioCommandRegistration.cpp
		src/Features/ScenarioCommandRegistration.hpp
		src/Features/SimulationCommands.cpp
		src/Features/SimulationCommands.hpp
		src/IO/SimulationEvents.cpp
		src/IO/SimulationEvents.hpp
		src/IO/System/EventEmitter.hpp
		src/IO/Commands/CreateMap.hpp
		src/IO/Commands/March.hpp
		src/IO/Commands/SpawnHunter.hpp
		src/IO/Commands/SpawnSwordsman.hpp
		src/IO/Events/MapCreated.hpp
		src/IO/Events/MarchEnded.hpp
		src/IO/Events/MarchStarted.hpp
		src/IO/Events/UnitAttacked.hpp
		src/IO/Events/UnitDied.hpp
		src/IO/Events/UnitMoved.hpp
		src/IO/Events/UnitSpawned.hpp
		src/IO/System/CommandParser.cpp
		src/IO/System/CommandParser.hpp
		src/IO/System/EventLog.hpp
		src/IO/System/PrintDebug.hpp
		src/IO/System/details/CommandParserVisitor.hpp
		src/IO/System/details/PrintFieldVisitor.hpp
)

target_compile_features(sw_battle_test PRIVATE cxx_std_20)
target_include_directories(sw_battle_test PRIVATE src/)

if(MSVC)
	target_compile_options(sw_battle_test PRIVATE /W4 /permissive-)
else()
	target_compile_options(sw_battle_test PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
