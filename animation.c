#include "animation.h"

static void animation_refresh_rect(Animation *anim) {
    anim->source_rect = (Rectangle){
        (float)(anim->x + anim->frame * anim->width),
        (float)anim->y,
        (float)anim->width,
        (float)anim->height
    };
}

Animation animation_create(float delay_ms, int frame_count, int x, int y, int width, int height) {
    //zero everything first
    Animation anim = { 0 };
    anim.delay = delay_ms;
    anim.frame_count = frame_count - 1;
    anim.x = x;
    anim.y = y;
    anim.width = width;
    anim.height = height;
    animation_refresh_rect(&anim);
    return anim;
}

void animation_update(Animation* anim, GameTime game_time) {
    //elapsed is in seconds so convert to milliseconds
    anim->elapsed += game_time.elapsed * 1000.0f;

    if (anim->elapsed >= anim->delay) {
        if (anim->frame >= anim->frame_count) {
            anim->frame = 0;
            anim->has_looped = true;
        } else {
            anim->frame++;
        }
        anim->elapsed -= anim->delay;
    }

    animation_refresh_rect(anim);
}

void animation_reset(Animation* anim) {
    anim->frame = 0;
    anim->has_looped = false;
    anim->elapsed = 0.0f;
    animation_refresh_rect(anim);
}

void animation_set_y_offset(Animation* anim, int y_offset) {
    anim->y = y_offset;
    animation_refresh_rect(anim);
}

void animation_set_frame(Animation* anim, int frame) {
    anim->frame = frame;
    animation_refresh_rect(anim);
}

bool animation_looped_once(const Animation* anim) {
    return anim->has_looped;
}