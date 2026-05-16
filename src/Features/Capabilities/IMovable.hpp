#pragma once

namespace sw::features
{
	class IMovable
	{
	public:
		virtual ~IMovable() = default;

		virtual int maxStepDistance() const = 0;
	};
}
