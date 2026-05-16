#include "Swordsman.hpp"

#include <Features/Common/MeleeAttackRule.hpp>
#include <Features/Common/MoveToTargetRule.hpp>
#include <Features/Stats.hpp>

namespace sw::features::swordsman
{
	Swordsman::Swordsman(const UnitId id, const Position pos, const int hp, const int strength) :
			state_{id, pos, 0, false},
			hp_(hp),
			maxHp_(hp)
	{
		stats_.emplace(std::string(stats::kStrength), strength);
		behavior_.addRule(std::make_unique<MeleeAttackRule>(stats::kStrength));
		behavior_.addRule(std::make_unique<MoveToTargetRule>());
	}

	UnitId Swordsman::id() const
	{
		return state_.id;
	}

	Position Swordsman::position() const
	{
		return state_.pos;
	}

	void Swordsman::setPosition(const Position pos)
	{
		state_.pos = pos;
	}

	int Swordsman::spawnOrder() const
	{
		return state_.spawnOrder;
	}

	void Swordsman::setSpawnOrder(const int order)
	{
		state_.spawnOrder = order;
	}

	std::string_view Swordsman::type() const
	{
		return "Swordsman";
	}

	const StatBlock& Swordsman::stats() const
	{
		return stats_;
	}

	bool Swordsman::pendingRemoval() const
	{
		return state_.pendingRemoval;
	}

	void Swordsman::markPendingRemoval()
	{
		state_.pendingRemoval = true;
	}

	bool Swordsman::act(ActionSession& session, io::EventEmitter& events)
	{
		return behavior_.act(id(), session, events);
	}

	void Swordsman::changeHp(const int delta)
	{
		if (delta < 0)
		{
			hp_ += delta;
			return;
		}
		if (delta > 0)
		{
			hp_ += delta;
			if (hp_ > maxHp_)
			{
				hp_ = maxHp_;
			}
		}
	}

	bool Swordsman::isDead() const
	{
		return hp_ <= 0;
	}

	int Swordsman::currentHp() const
	{
		return hp_;
	}

	int Swordsman::maxStepDistance() const
	{
		return 1;
	}

	void Swordsman::setMarchTarget(const Position target)
	{
		marchTarget_ = target;
	}

	std::optional<Position> Swordsman::getMarchTarget() const
	{
		return marchTarget_;
	}

	void Swordsman::clearMarchTarget()
	{
		marchTarget_.reset();
	}
}
