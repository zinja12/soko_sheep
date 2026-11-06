#include "title_screen.h"

void title_update(TitleScreen* title, GameTime game_time, const Input* input) {

    //update sheep
    for (int i = 0; i < world->sheep_count; i++) {
        Sheep *sheep = &world->sheep[i];
        sheep_update(sheep, game_time);
    }
}

ScreenTarget title_as_screen_target(TitleScreen *title) {
    return (ScreenTarget){ title, title_draw };
}

void title_draw(void *self) {

}