#pragma once

#include <Core/IUnit.hpp>
#include <Features/Capabilities/IDamageable.hpp>
#include <Features/Capabilities/IMarchable.hpp>
#include <Features/Capabilities/IMovable.hpp>
#include <Features/Common/BehaviorSequence.hpp>

#include <optional>

namespace sw::features::swordsman
{
	class Swordsman final :
			public IUnit,
			public IDamageable,
			public IMovable,
			public IMarchable
	{
	public:
		Swordsman(UnitId id, Position pos, int hp, int strength);

		UnitId id() const override;
		Position position() const override;
		void setPosition(Position pos) override;

		int spawnOrder() const override;
		void setSpawnOrder(int order) override;

		std::string_view type() const override;
		const StatBlock& stats() const override;

		bool pendingRemoval() const override;
		void markPendingRemoval() override;

		bool act(ActionSession& session, io::EventEmitter& events) override;

		void changeHp(int delta) override;
		bool isDead() const override;
		int currentHp() const override;

		int maxStepDistance() const override;

		void setMarchTarget(Position target) override;
		std::optional<Position> getMarchTarget() const override;
		void clearMarchTarget() override;

	private:
		UnitState state_;
		StatBlock stats_;
		int hp_{};
		int maxHp_{};
		std::optional<Position> marchTarget_;
		BehaviorSequence behavior_;
	};
}
