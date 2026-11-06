#ifndef SCREEN_CANVAS_H
#define SCREEN_CANVAS_H

#include "raylib.h"
#include "screen_target.h"

typedef struct ScreenCanvas {
    RenderTexture2D target;
    int base_width, base_height;
    Color background_color;
} ScreenCanvas;

void canvas_init(ScreenCanvas* canvas, int base_width, int base_height, Color background_color);
void canvas_unload(ScreenCanvas* canvas);
float canvas_get_scale(const ScreenCanvas *canvas);
Vector2 canvas_screen_to_virtual(const ScreenCanvas *canvas, Vector2 screen_position);
void canvas_draw(const ScreenCanvas *canvas, ScreenTarget screen_target);

#endif