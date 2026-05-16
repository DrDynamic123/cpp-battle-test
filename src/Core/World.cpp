#include "World.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace sw
{
	namespace
	{
		std::string unitErrorPrefix(const UnitId id)
		{
			return "Unit " + std::to_string(id.value) + ": ";
		}
	}

	void World::createMap(const int width, const int height)
	{
		if (mapCreated_)
		{
			throw std::runtime_error("Map has already been created");
		}
		if (width <= 0 || height <= 0)
		{
			throw std::runtime_error("Map dimensions must be positive");
		}

		width_ = width;
		height_ = height;
		mapCreated_ = true;
	}

	bool World::hasMap() const noexcept
	{
		return mapCreated_;
	}

	int World::width() const noexcept
	{
		return width_;
	}

	int World::height() const noexcept
	{
		return height_;
	}

	void World::spawnUnit(std::unique_ptr<IUnit> unit)
	{
		validateMapReady();
		if (!unit)
		{
			throw std::runtime_error("Cannot spawn null unit");
		}
		const auto id = unit->id();
		if (contains(id))
		{
			throw std::runtime_error("Duplicate unit id: " + std::to_string(id.value));
		}
		validateInside(unit->position(), "spawn position");
		if (occupiesCell(*unit))
		{
			if (isCellOccupied(unit->position()))
			{
				throw std::runtime_error(unitErrorPrefix(id) + "spawn cell is already occupied");
			}
			setCellOccupied(unit->position(), id);
		}

		const auto [_, inserted] = units_.emplace(id, std::move(unit));
		if (!inserted)
		{
			throw std::runtime_error("Duplicate unit id: " + std::to_string(id.value));
		}
	}

	void World::removeUnit(const UnitId id)
	{
		auto it = units_.find(id);
		if (it == units_.end())
		{
			return;
		}

		const auto& unit = *it->second;
		if (occupiesCell(unit))
		{
			const auto occupant = cellOccupant(unit.position());
			if (occupant.has_value() && occupant.value() == id)
			{
				clearCell(unit.position());
			}
		}
		units_.erase(it);
	}

	void World::relocateUnit(const UnitId id, const Position to)
	{
		auto& unit = getUnit(id);
		validateInside(to, "relocation target");
		if (unit.position() == to)
		{
			return;
		}

		if (occupiesCell(unit))
		{
			if (isCellOccupied(to))
			{
				throw std::runtime_error(unitErrorPrefix(id) + "relocation target cell is occupied");
			}
			clearCell(unit.position());
			setCellOccupied(to, id);
		}

		unit.setPosition(to);
	}

	bool World::contains(const UnitId id) const noexcept
	{
		return units_.find(id) != units_.end();
	}

	bool World::isInside(const Position pos) const noexcept
	{
		return mapCreated_ && pos.x >= 0 && pos.y >= 0 && pos.x < width_ && pos.y < height_;
	}

	bool World::isCellOccupied(const Position pos) const
	{
		return cellOccupant(pos).has_value();
	}

	std::optional<UnitId> World::cellOccupant(const Position pos) const
	{
		if (!isInside(pos))
		{
			return std::nullopt;
		}

		const auto it = occupied_.find(pos);
		if (it == occupied_.end())
		{
			return std::nullopt;
		}
		return it->second;
	}

	void World::setCellOccupied(const Position pos, const UnitId id)
	{
		validateInside(pos, "occupied cell");
		const auto [_, inserted] = occupied_.emplace(pos, id);
		if (!inserted)
		{
			throw std::runtime_error("Cell is already occupied");
		}
	}

	void World::clearCell(const Position pos)
	{
		occupied_.erase(pos);
	}

	IUnit& World::getUnit(const UnitId id)
	{
		auto it = units_.find(id);
		if (it == units_.end())
		{
			throw std::runtime_error("Unknown unit id: " + std::to_string(id.value));
		}
		return *it->second;
	}

	const IUnit& World::getUnit(const UnitId id) const
	{
		auto it = units_.find(id);
		if (it == units_.end())
		{
			throw std::runtime_error("Unknown unit id: " + std::to_string(id.value));
		}
		return *it->second;
	}

	std::size_t World::unitCount() const noexcept
	{
		return units_.size();
	}

	std::vector<UnitId> World::unitOrder() const
	{
		std::vector<UnitId> ids;
		ids.reserve(units_.size());
		for (const auto& [id, _] : units_)
		{
			ids.push_back(id);
		}

		std::sort(
			ids.begin(),
			ids.end(),
			[this](const UnitId lhs, const UnitId rhs)
			{
				return getUnit(lhs).spawnOrder() < getUnit(rhs).spawnOrder();
			});
		return ids;
	}

	void World::validateMapReady() const
	{
		if (!mapCreated_)
		{
			throw std::runtime_error("Map has not been created");
		}
	}

	void World::validateInside(const Position pos, const char* const what) const
	{
		if (!isInside(pos))
		{
			throw std::runtime_error(std::string(what) + " is outside the map");
		}
	}

	bool World::occupiesCell(const IUnit& unit) noexcept
	{
		return unit.occupiesCell();
	}
}
