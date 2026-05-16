#pragma once

namespace sw::features
{
	class IDamageable
	{
	public:
		virtual ~IDamageable() = default;

		// delta < 0 — урон, delta > 0 — лечение (не выше максимального HP)
		virtual void changeHp(int delta) = 0;
		virtual bool isDead() const = 0;
		virtual int currentHp() const = 0;
	};
}
