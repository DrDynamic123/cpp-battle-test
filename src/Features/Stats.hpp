#pragma once

#include <Core/Stats.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace sw::features::stats
{
	inline constexpr std::string_view kStrength = "strength";
	inline constexpr std::string_view kAgility = "agility";
	inline constexpr std::string_view kRange = "range";
	inline constexpr std::string_view kSpirit = "spirit";
	inline constexpr std::string_view kPower = "power";

	inline int get(const StatBlock& stats, const std::string_view key)
	{
		const auto it = stats.find(std::string(key));
		if (it == stats.end())
		{
			throw std::runtime_error("Missing stat: " + std::string(key));
		}
		return it->second;
	}
}
