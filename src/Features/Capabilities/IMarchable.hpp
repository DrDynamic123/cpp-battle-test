#pragma once

#include <Core/Position.hpp>

#include <optional>

namespace sw::features
{
	class IMarchable
	{
	public:
		virtual ~IMarchable() = default;

		virtual void setMarchTarget(Position target) = 0;
		virtual std::optional<Position> getMarchTarget() const = 0;
		virtual void clearMarchTarget() = 0;
	};
}
