#pragma once

namespace sw
{
	struct Position
	{
		int x{};
		int y{};
	};

	inline bool operator==(const Position lhs, const Position rhs) noexcept
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	inline bool operator!=(const Position lhs, const Position rhs) noexcept
	{
		return !(lhs == rhs);
	}

	inline bool operator<(const Position lhs, const Position rhs) noexcept
	{
		if (lhs.y != rhs.y)
		{
			return lhs.y < rhs.y;
		}
		return lhs.x < rhs.x;
	}

}
