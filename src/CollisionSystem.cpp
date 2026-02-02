#include "CollisionSystem.h"
#include "Entity.h"
#include <cmath>

SpatialHash::SpatialHash(float cellSize)
    : m_cellSize(cellSize)
{
}

void SpatialHash::Clear()
{
    m_grid.clear();
}

void SpatialHash::Insert(Entity* entity)
{
    if (!entity) return;

    int32_t cellX, cellY;
    GetCellCoords(entity->GetPosition(), cellX, cellY);

    int64_t key = HashCell(cellX, cellY);
    m_grid[key].push_back(entity);
}

std::vector<Entity*> SpatialHash::QueryRadius(Vector2 position, float radius)
{
    std::vector<Entity*> results;

    int32_t centerX, centerY;
    GetCellCoords(position, centerX, centerY);

    // Calculate how many cells to check based on radius
    int32_t cellRadius = static_cast<int32_t>(std::ceil(radius / m_cellSize));

    // Check all cells within the radius
    for (int32_t dy = -cellRadius; dy <= cellRadius; ++dy)
    {
        for (int32_t dx = -cellRadius; dx <= cellRadius; ++dx)
        {
            int64_t key = HashCell(centerX + dx, centerY + dy);

            auto it = m_grid.find(key);
            if (it != m_grid.end())
            {
                // Add all entities in this cell to results
                results.insert(results.end(), it->second.begin(), it->second.end());
            }
        }
    }

    return results;
}

int64_t SpatialHash::HashCell(int32_t x, int32_t y) const
{
    // Combine x and y into a single 64-bit key
    // Upper 32 bits = x, lower 32 bits = y
    return (static_cast<int64_t>(x) << 32) | (static_cast<int64_t>(y) & 0xFFFFFFFF);
}

void SpatialHash::GetCellCoords(Vector2 pos, int32_t& outX, int32_t& outY) const
{
    outX = static_cast<int32_t>(std::floor(pos.x / m_cellSize));
    outY = static_cast<int32_t>(std::floor(pos.y / m_cellSize));
}
