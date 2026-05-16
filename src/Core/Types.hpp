#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace sw
{
	struct UnitId
	{
		uint32_t value{};
	};

	inline bool operator==(const UnitId lhs, const UnitId rhs) noexcept
	{
		return lhs.value == rhs.value;
	}

	inline bool operator!=(const UnitId lhs, const UnitId rhs) noexcept
	{
		return !(lhs == rhs);
	}

	inline bool operator<(const UnitId lhs, const UnitId rhs) noexcept
	{
		return lhs.value < rhs.value;
	}

	struct UnitIdHash
	{
		std::size_t operator()(const UnitId id) const noexcept
		{
			return std::hash<uint32_t>{}(id.value);
		}
	};
}
