#include "screen_canvas.h"
#include <math.h>

void canvas_init(ScreenCanvas* canvas, int base_width, int base_height, Color background_color) {
    canvas->base_width = base_width;
    canvas->base_height = base_height;
    canvas->background_color = background_color;
    //render targets live on gpu, free with unload after initwindow()
    canvas->target = LoadRenderTexture(base_width, base_height);
    SetTextureFilter(canvas->target.texture, TEXTURE_FILTER_POINT);
}

void canvas_unload(ScreenCanvas *canvas) {
    UnloadRenderTexture(canvas->target);
}

float canvas_get_scale(const ScreenCanvas *canvas) {
    float scale_x = (float)GetScreenWidth() / canvas->base_width;
    float scale_y = (float)GetScreenHeight() / canvas->base_height;
    //hold aspect ratio
    return fminf(scale_x, scale_y);
}

//where scaled canvas sits in the window (centered with letterboxing)
static Rectangle canvas_dest_rect(const ScreenCanvas *canvas) {
    float scale = canvas_get_scale(canvas);
    float width = canvas->base_width * scale;
    float height = canvas->base_height * scale;
    return (Rectangle){
        (GetScreenWidth() - width) / 2.0f,
        (GetScreenHeight() - height) / 2.0f,
        width,
        height
    };
}

//window pixels to virtual canvas pixels so the game does not have to worry about window size
Vector2 canvas_screen_to_virtual(const ScreenCanvas *canvas, Vector2 screen_position) {
    Rectangle dest = canvas_dest_rect(canvas);
    float scale = canvas_get_scale(canvas);
    return (Vector2){
        (screen_position.x - dest.x) / scale,
        (screen_position.y - dest.y) / scale
    };
}

void canvas_draw(const ScreenCanvas *canvas, ScreenTarget screen_target) {
    //draw scene into render texture
    BeginTextureMode(canvas->target);
    ClearBackground(canvas->background_color);
    //call target draw via function pointer
    screen_target.draw(screen_target.self);
    EndTextureMode();

    //draw texture scaled up to window
    BeginDrawing();
    ClearBackground(BLACK);
    //open gl stores render textures upside down for some reason
    //source height is negative to flip it to the correct side
    Rectangle source = { 0, 0, (float)canvas->target.texture.width, -(float)canvas->target.texture.height };
    DrawTexturePro(canvas->target.texture, source, canvas_dest_rect(canvas), (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}