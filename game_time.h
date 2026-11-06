#ifndef GAME_TIME_H
#define GAME_TIME_H

#include <math.h>

//recreation of Monogame/XNA GameTime
typedef struct GameTime {
    //seconds since previous update
    float elapsed;
    //seconds since game started
    double total;
} GameTime;

static inline float frame_lerp(float amount_per_frame, float dt) {
    return 1.0f - powf(1.0f - amount_per_frame, dt * 60.0f);
}

#endif