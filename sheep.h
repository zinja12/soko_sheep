#ifndef SHEEP_H
#define SHEEP_H

#include "animation.h"
#include "game_time.h"
#include "raylib.h"

typedef enum SheepState {
    SHEEP_REST,
    SHEEP_PUSHED,
    SHEEP_SLIDE
} SheepState;

typedef struct Sheep {
    int tile_x, tile_y;
    Vector2 from_position, to_position, position;
    float move_time, move_duration;
    bool moving;

    //directions
    int dx, dy;
    SheepState state;
    bool in_air;

    bool is_ram;

    int current_frame;
    bool flip;

    Animation animation;
} Sheep;

void sheep_init(Sheep *sheep, int tile_x, int tile_y, bool is_ram);
void sheep_start_step(Sheep *sheep, int dx, int dy, float duration);
Vector2 tile_to_position(int tile_x, int tile_y);
//return true when sheep reaches target tile
bool sheep_update(Sheep *sheep, GameTime game_time);
void sheep_draw(Sheep *sheep);

#endif