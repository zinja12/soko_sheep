#ifndef TILE_H
#define TILE_H

#include "animation.h"
#include "constants.h"
#include "game_time.h"

typedef enum TileType {
    VOID,
    GRASS,
    WATER,
    TREE,
    PEN,
    FLOWER,
    WOLF,
    THISTLE,
    CHICKEN
} TileType;

typedef struct Tile {
    int x, y, source_x, source_y;
    bool flip, animated;
    TileType tile_type;
    Rectangle source_rect;

    Animation tile_animation;
} Tile;

typedef struct TileAnimationData {
    int frame_count;
    float speed;
    int source_x, source_y, size;
} TileAnimationData;

void tile_init(Tile *tile, TileType tile_type, Vector2 position, bool flip);
void tile_init_animation(Tile *tile, bool animated, int frame_count, float speed, int source_x, int source_y, int size);
void tile_animation_update(Tile *tile, GameTime game_time);
Vector2 get_tile_source_coordinates(TileType tile_type);
bool get_animated(TileType tile_type);
TileAnimationData get_tile_animation_data(TileType tile_type);
bool tile_blocks_dog(TileType tile_type);
bool tile_blocks_sheep(TileType tile_type);
void tile_draw(Tile *tile);

#endif