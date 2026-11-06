#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "constants.h"
#include "game_time.h"
#include "input.h"
#include "screen_target.h"
#include "sheep.h"
#include "tile.h"

typedef struct TitleScreen {
    Tile *tiles;
    int width, height;

    Sheep sheep[MAX_SHEEP];
} TitleScreen;

void title_update(TitleScreen* title, GameTime game_time, const Input* input);
void title_draw(void *self);
ScreenTarget title_as_screen_target(TitleScreen *title);

#endif