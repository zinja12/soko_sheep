#ifndef END_SCREEN_H
#define END_SCREEN_H

#include "animation.h"
#include "game_time.h"
#include "input.h"
#include "screen_target.h"
#include "sheep.h"
#include "tile.h"

typedef struct EndScreen {
    Tile tiles[SCREEN_TILE_SIZE][SCREEN_TILE_SIZE];
    Sheep sheep;

    Animation z_animation;
    float fade_time;
} EndScreen;

void end_init(EndScreen *end);
bool end_update(EndScreen *end, GameTime game_time, const Input *input);
void end_draw(void *self);
ScreenTarget end_as_screen_target(EndScreen *end);

#endif