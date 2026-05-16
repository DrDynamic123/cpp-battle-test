#include "Actions.hpp"

#include <Core/ActionSession.hpp>
#include <Core/World.hpp>
#include <Features/Capabilities/IDamageable.hpp>
#include <Features/Capabilities/IMarchable.hpp>
#include <Features/Capabilities/IMovable.hpp>
#include <Features/Common/Navigation.hpp>
#include <IO/SimulationEvents.hpp>

#include <algorithm>
#include <stdexcept>

namespace sw::features::actions
{
	namespace
	{
		uint32_t clampHpForEvent(const int hp) noexcept
		{
			return static_cast<uint32_t>(std::max(0, hp));
		}
	}

	void dealDamage(
		ActionSession& session,
		io::EventEmitter& events,
		const UnitId attacker,
		const UnitId target,
		const int amount)
	{
		if (amount <= 0)
		{
			throw std::runtime_error("Damage must be positive");
		}

		auto& world = session.world();
		auto& targetUnit = world.getUnit(target);
		auto* damageable = dynamic_cast<IDamageable*>(&targetUnit);
		if (damageable == nullptr)
		{
			throw std::runtime_error("Target unit is not damageable");
		}

		damageable->changeHp(-amount);
		io::events::unitAttacked(
			events,
			session.tick(),
			attacker,
			target,
			static_cast<uint32_t>(amount),
			clampHpForEvent(damageable->currentHp()));

		if (damageable->isDead() && !targetUnit.pendingRemoval())
		{
			targetUnit.markPendingRemoval();
			io::events::unitDied(events, session.tick(), target);
		}
	}

	void healUnit(
		ActionSession& session,
		io::EventEmitter&,
		const UnitId,
		const UnitId target,
		const int amount)
	{
		if (amount <= 0)
		{
			throw std::runtime_error("Heal amount must be positive");
		}

		auto* damageable = dynamic_cast<IDamageable*>(&session.getUnit(target));
		if (damageable == nullptr)
		{
			throw std::runtime_error("Target unit is not damageable");
		}
		damageable->changeHp(amount);
	}

	void moveUnit(ActionSession& session, io::EventEmitter& events, const UnitId id, const Position to)
	{
		auto& world = session.world();
		auto& unit = world.getUnit(id);
		auto* movable = dynamic_cast<IMovable*>(&unit);
		if (movable == nullptr)
		{
			throw std::runtime_error("Unit is not movable");
		}
		if (!world.isInside(to))
		{
			throw std::runtime_error("Move target is outside the map");
		}
		if (unit.position() == to)
		{
			return;
		}
		if (navigation::chebyshevDistance(unit.position(), to) > movable->maxStepDistance())
		{
			throw std::runtime_error("Move distance is too large");
		}

		world.relocateUnit(id, to);
		io::events::unitMoved(events, session.tick(), id, to);
	}

	void endMarch(ActionSession& session, io::EventEmitter& events, const UnitId id)
	{
		auto* marchable = dynamic_cast<IMarchable*>(&session.getUnit(id));
		if (marchable == nullptr)
		{
			throw std::runtime_error("Unit is not marchable");
		}

		const auto pos = session.world().getUnit(id).position();
		marchable->clearMarchTarget();
		io::events::marchEnded(events, session.tick(), id, pos);
	}

	void scheduleRemoval(ActionSession& session, io::EventEmitter& events, const UnitId id)
	{
		auto& unit = session.world().getUnit(id);
		if (!unit.pendingRemoval())
		{
			unit.markPendingRemoval();
			io::events::unitDied(events, session.tick(), id);
		}
	}
}
