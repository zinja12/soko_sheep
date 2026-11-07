#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "animation.h"
#include "game_time.h"
#include "input.h"
#include "screen_target.h"
#include "sheep.h"
#include "tile.h"

#define SCREEN_TILE_SIZE 10

typedef struct TitleScreen {
    Tile tiles[10][10];
    int width, height;

    Sheep sheep[2];
    int sheep_count;
    
    Animation z_animation;
} TitleScreen;

bool title_init(TitleScreen* title);
bool title_update(TitleScreen* title, GameTime game_time, const Input* input);
void title_draw(void *self);
ScreenTarget title_as_screen_target(TitleScreen *title);

#endif