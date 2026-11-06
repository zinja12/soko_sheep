#ifndef SHAKE_H
#define SHAKE_H

#include "raylib.h"
#include "game_time.h"

typedef struct ShakeEffect {
    float timer, duration, strength;
    Vector2 offset;
} ShakeEffect;

void shake_trigger(ShakeEffect* shake, float intensity, float duration);
void shake_update(ShakeEffect* shake, GameTime game_time);
bool shake_is_shaking(const ShakeEffect* shake);

//raylib camera 2d for access to camera transforms
Camera2D shake_get_camera(const ShakeEffect* shake);

#endif