#pragma once

#include "IUnit.hpp"
#include "Position.hpp"
#include "Types.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace sw
{
	class World
	{
	public:
		World() = default;

		void createMap(int width, int height);
		[[nodiscard]] bool hasMap() const noexcept;
		[[nodiscard]] int width() const noexcept;
		[[nodiscard]] int height() const noexcept;

		void spawnUnit(std::unique_ptr<IUnit> unit);
		void removeUnit(UnitId id);
		void relocateUnit(UnitId id, Position to);

		[[nodiscard]] bool contains(UnitId id) const noexcept;
		[[nodiscard]] bool isInside(Position pos) const noexcept;
		[[nodiscard]] bool isCellOccupied(Position pos) const;
		[[nodiscard]] std::optional<UnitId> cellOccupant(Position pos) const;

		IUnit& getUnit(UnitId id);
		const IUnit& getUnit(UnitId id) const;

		[[nodiscard]] std::size_t unitCount() const noexcept;
		[[nodiscard]] std::vector<UnitId> unitOrder() const;

	private:
		void setCellOccupied(Position pos, UnitId id);
		void clearCell(Position pos);
		void validateMapReady() const;
		void validateInside(Position pos, const char* what) const;
		[[nodiscard]] static bool occupiesCell(const IUnit& unit) noexcept;

		int width_{};
		int height_{};
		bool mapCreated_{false};
		std::map<UnitId, std::unique_ptr<IUnit>> units_;
		std::map<Position, UnitId> occupied_;
	};
}
