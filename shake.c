#include "shake.h"
#include <math.h>

void shake_trigger(ShakeEffect* shake, float intensity, float duration) {
    if (duration <= 0.0f || intensity <= 0.0f) {
        return;
    }
    shake->strength = intensity;
    shake->duration = duration;
    shake->timer = duration;
}

void shake_update(ShakeEffect* shake, GameTime game_time) {
    if (shake->timer <= 0.0f) {
        shake->offset = (Vector2) { 0, 0 };
        return;
    }

    shake->timer -= game_time.elapsed;

    //normalize remaining fraction in [0, 1]
    float t = (shake->duration > 0.0f) ? (shake->timer / shake->duration) : 0.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // ease-out (quadratic) so shake decays smoothly
    float current_intensity = shake->strength * (t * t);

    //use get random value for angles
    float angle = GetRandomValue(0, 359) * DEG2RAD;
    shake->offset = (Vector2){ cosf(angle) * current_intensity, sinf(angle) * current_intensity };

    if (shake->timer <= 0.0f) {
        shake->timer = 0.0f;
        shake->offset = (Vector2){ 0, 0 };
    }
}

bool shake_is_shaking(const ShakeEffect* shake) {
    return shake->timer > 0.0f;
}

Camera2D shake_get_camera(const ShakeEffect* shake) {
    Camera2D camera = { 0 };
    //move everything inside BeginMode2D by the offset
    camera.offset = shake->offset;
    camera.zoom = 1.0f;
    return camera;
}