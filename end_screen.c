#include "assets.h"
#include "end_screen.h"

void end_init(EndScreen *end) {
    *end = (EndScreen) { 0 };

    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            if (y == 0 || y == SCREEN_TILE_SIZE - 1) {
                tile_init(&end->tiles[y][x], TREE, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);    
            } else if (x == 0 || x == SCREEN_TILE_SIZE - 1) {
                tile_init(&end->tiles[y][x], TREE, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);    
            } else {
                tile_init(&end->tiles[y][x], GRASS, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);
            }
        }
    }

    //init sheep
    sheep_init(&end->sheep, 6, 4, false);

    end->z_animation = animation_create(450.0f, 3, 1 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

bool end_update(EndScreen *end, GameTime game_time, const Input *input) {
    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            tile_animation_update(&end->tiles[y][x], game_time);
        }
    }

    sheep_update(&end->sheep, game_time);
    animation_update(&end->z_animation, game_time);

    if (end->fade_time < FADE_TIME) {
        end->fade_time += game_time.elapsed;
        return false;
    }

    return input->undo;
}

ScreenTarget end_as_screen_target(EndScreen *end) {
    return (ScreenTarget){ end, end_draw };
}

void end_draw(void *self) {
    EndScreen *end = (EndScreen *)self;

    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            tile_draw(&end->tiles[y][x]);
        }
    }

    sheep_draw(&end->sheep);

    Rectangle e_source = (Rectangle) { 4 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle n_source = (Rectangle) { 5 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle d_source = (Rectangle) { 6 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

    Rectangle dog_source = (Rectangle) { 1 * TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

    Rectangle e_destination = (Rectangle) { 3 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle n_destination = (Rectangle) { 4 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle d_destination = (Rectangle) { 5 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };

    //sleeping dog with zs above it
    Rectangle dog_destination = (Rectangle) { 4 * TILE_DRAW_SIZE, 6 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle z_destination = (Rectangle) { 4 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };

    Vector2 origin = (Vector2) { 0, 0 };

    DrawTexturePro(image_tileset, e_source, e_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, n_source, n_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, d_source, d_destination, origin, 0.0f, WHITE);

    DrawTexturePro(image_tileset, dog_source, dog_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, end->z_animation.source_rect, z_destination, origin, 0.0f, WHITE);

    DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, Fade(BLACK, 1.0f - end->fade_time / FADE_TIME));
}