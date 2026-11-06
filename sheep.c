#include "assets.h"
#include "constants.h"
#include "raymath.h"
#include "sheep.h"

#define SHEEP_SOURCE_X 4
#define SHEEP_SOURCE_Y 5
#define SHEEP_FRAME_COUNT 2

#define RAM_SOURCE_X 6

#define SHEEP_IDLE_DELAY 350.0f
#define SHEEP_MOVE_DELAY 100.0f

Vector2 tile_to_position(int tile_x, int tile_y) {
    return (Vector2) { (float)(tile_x * TILE_DRAW_SIZE), (float)(tile_y * TILE_DRAW_SIZE) };
}

void sheep_init(Sheep *sheep, int tile_x, int tile_y, bool is_ram) {
    *sheep = (Sheep) { 0 };
    sheep->tile_x = tile_x;
    sheep->tile_y = tile_y;
    sheep->is_ram = is_ram;
    int source_x = is_ram ? RAM_SOURCE_X : SHEEP_SOURCE_X;
    sheep->position = tile_to_position(tile_x, tile_y);
    sheep->animation = animation_create(SHEEP_IDLE_DELAY, SHEEP_FRAME_COUNT, source_x * TILE_SIZE, SHEEP_SOURCE_Y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

void sheep_start_step(Sheep *sheep, int dx, int dy, float duration) {
    sheep->dx = dx;
    sheep->dy = dy;
    sheep->tile_x += dx;
    sheep->tile_y += dy;
    sheep->from_position = sheep->position;
    sheep->to_position = tile_to_position(sheep->tile_x, sheep->tile_y);
    sheep->move_time = 0.0f;
    sheep->move_duration = duration;
    sheep->moving = true;

    if (dx != 0) {
        sheep->flip = dx < 0;
    }
}

bool sheep_update(Sheep *sheep, GameTime game_time) {
    sheep->animation.delay = sheep->moving ? SHEEP_MOVE_DELAY : SHEEP_IDLE_DELAY;
    animation_update(&sheep->animation, game_time);
    
    if (!sheep->moving) {
        return false;
    }

    sheep->move_time += game_time.elapsed;
    float t = fminf(sheep->move_time / sheep->move_duration, 1.0f);
    Vector2 position = Vector2Lerp(sheep->from_position, sheep->to_position, t);
    //whole pixels only
    sheep->position = (Vector2){ roundf(position.x), roundf(position.y) };

    if (t >= 1.0f) {
        sheep->position = sheep->to_position;
        sheep->moving = false;
        return true;
    }
    return false;
}

void sheep_draw(Sheep *sheep) {
    Rectangle source = sheep->animation.source_rect;
    if (sheep->flip) source.width = -source.width;
    Rectangle destination = { sheep->position.x, sheep->position.y, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    DrawTexturePro(image_tileset, source, destination, (Vector2){ 0, 0 }, 0.0f, WHITE);
}