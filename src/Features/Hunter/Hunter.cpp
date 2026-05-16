#include "Hunter.hpp"

#include <Features/Common/MeleeAttackRule.hpp>
#include <Features/Common/MoveToTargetRule.hpp>
#include <Features/Common/RangedAttackRule.hpp>
#include <Features/Stats.hpp>

namespace sw::features::hunter
{
	Hunter::Hunter(
		const UnitId id,
		const Position pos,
		const int hp,
		const int agility,
		const int strength,
		const int range) :
			state_{id, pos, 0, false},
			hp_(hp),
			maxHp_(hp)
	{
		stats_.reserve(3);
		stats_.emplace(std::string(stats::kAgility), agility);
		stats_.emplace(std::string(stats::kStrength), strength);
		stats_.emplace(std::string(stats::kRange), range);
		behavior_.addRule(std::make_unique<RangedAttackRule>(stats::kAgility, 2, stats::kRange, true));
		behavior_.addRule(std::make_unique<MeleeAttackRule>(stats::kStrength));
		behavior_.addRule(std::make_unique<MoveToTargetRule>());
	}

	UnitId Hunter::id() const
	{
		return state_.id;
	}

	Position Hunter::position() const
	{
		return state_.pos;
	}

	void Hunter::setPosition(const Position pos)
	{
		state_.pos = pos;
	}

	int Hunter::spawnOrder() const
	{
		return state_.spawnOrder;
	}

	void Hunter::setSpawnOrder(const int order)
	{
		state_.spawnOrder = order;
	}

	std::string_view Hunter::type() const
	{
		return "Hunter";
	}

	const StatBlock& Hunter::stats() const
	{
		return stats_;
	}

	bool Hunter::pendingRemoval() const
	{
		return state_.pendingRemoval;
	}

	void Hunter::markPendingRemoval()
	{
		state_.pendingRemoval = true;
	}

	bool Hunter::act(ActionSession& session, io::EventEmitter& events)
	{
		return behavior_.act(id(), session, events);
	}

	void Hunter::changeHp(const int delta)
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

	bool Hunter::isDead() const
	{
		return hp_ <= 0;
	}

	int Hunter::currentHp() const
	{
		return hp_;
	}

	int Hunter::maxStepDistance() const
	{
		return 1;
	}

	void Hunter::setMarchTarget(const Position target)
	{
		marchTarget_ = target;
	}

	std::optional<Position> Hunter::getMarchTarget() const
	{
		return marchTarget_;
	}

	void Hunter::clearMarchTarget()
	{
		marchTarget_.reset();
	}
}
