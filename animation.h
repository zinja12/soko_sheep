#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"
#include "game_time.h"

typedef struct Animation {
    //current frame's rectangle on spritesheet
    Rectangle source_rect;
    //top left coordinates of the first frame in pixels
    int x, y;
    //size of one frame
    int width, height;
    //index of the last frame (frame_count - 1)
    int frame_count;

    //milliseconds since last frame change
    float elapsed;
    //milliseconds per frame
    float delay;
    //current frame index
    int frame;
    //has animation looped
    bool has_looped;
} Animation;

// Returns the struct by value -- there is no `new`, the caller just stores it.
Animation animation_create(float delay_ms, int frame_count, int x, int y, int width, int height);

void animation_update(Animation* anim, GameTime game_time);
void animation_reset(Animation* anim);
void animation_set_y_offset(Animation* anim, int y_offset);
void animation_set_frame(Animation* anim, int frame);
bool animation_looped_once(const Animation* anim);

#endif