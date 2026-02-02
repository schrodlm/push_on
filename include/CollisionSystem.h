#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "raylib.h"

// Forward declaration
class Entity;

// Collision layer definitions using bitflags
// Each entity can belong to one or more layers
enum CollisionLayer : uint32_t {
    LAYER_NONE           = 0,
    LAYER_PLAYER_1       = 1 << 0,   // 0x0001
    LAYER_PLAYER_2       = 1 << 1,   // 0x0002
    LAYER_PLAYER_3       = 1 << 2,   // 0x0004
    LAYER_PLAYER_4       = 1 << 3,   // 0x0008
    LAYER_ENEMY          = 1 << 4,   // 0x0010
    LAYER_PLAYER_ATTACK  = 1 << 5,   // 0x0020 - Bullets, abilities from players
    LAYER_ENEMY_ATTACK   = 1 << 6,   // 0x0040 - Bullets, abilities from enemies
    LAYER_NEUTRAL_HAZARD = 1 << 7,   // 0x0080 - Environmental damage (hits everyone)
    LAYER_PICKUP         = 1 << 8,   // 0x0100 - Items, power-ups

    // Helper combinations
    LAYER_ALL_PLAYERS = LAYER_PLAYER_1 | LAYER_PLAYER_2 | LAYER_PLAYER_3 | LAYER_PLAYER_4,
    LAYER_ALL = 0xFFFFFFFF
};

/**
 * Spatial hash grid for efficient broad-phase collision detection.
 * Divides the world into cells and only checks entities in nearby cells.
 * This reduces collision checks from O(n²) to approximately O(n).
 */
class SpatialHash {
public:
    /**
     * @param cellSize Size of each grid cell in pixels (default 100.0f)
     *                 Larger cells = fewer cells but more entities per cell
     *                 Smaller cells = more cells but fewer entities per cell
     */
    explicit SpatialHash(float cellSize = 100.0f);

    /**
     * Clear all entities from the spatial hash.
     * Call this at the beginning of each collision detection frame.
     */
    void Clear();

    /**
     * Insert an entity into the spatial hash based on its position.
     * @param entity Pointer to entity to insert
     */
    void Insert(Entity* entity);

    /**
     * Query all entities within a radius of a position.
     * Checks the cell containing the position plus all 8 neighboring cells.
     * @param position Center position to query
     * @param radius Search radius
     * @return Vector of entity pointers in the queried area
     */
    std::vector<Entity*> QueryRadius(Vector2 position, float radius);

private:
    float m_cellSize;
    std::unordered_map<int64_t, std::vector<Entity*>> m_grid;

    /**
     * Hash a cell coordinate to a 64-bit integer key.
     * @param x Cell x coordinate
     * @param y Cell y coordinate
     * @return 64-bit hash key
     */
    int64_t HashCell(int32_t x, int32_t y) const;

    /**
     * Convert a world position to cell coordinates.
     * @param pos World position
     * @param outX Output cell x coordinate
     * @param outY Output cell y coordinate
     */
    void GetCellCoords(Vector2 pos, int32_t& outX, int32_t& outY) const;
};
