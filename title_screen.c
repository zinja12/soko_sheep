#include "assets.h"
#include "title_screen.h"

static inline Tile *title_get_tile(TitleScreen *title, int x, int y) {
    if (x < 0 || y < 0 || x >= SCREEN_TILE_SIZE || y >= SCREEN_TILE_SIZE) {
        return 0;
    }
    return &title->tiles[y][x];
}

bool title_init(TitleScreen *title) {
    *title = (TitleScreen) { 0 };

    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            if (y == 0 || y == SCREEN_TILE_SIZE - 1) {
                tile_init(&title->tiles[y][x], TREE, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);    
            } else if (x == 0 || x == SCREEN_TILE_SIZE - 1) {
                tile_init(&title->tiles[y][x], TREE, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);    
            } else {
                tile_init(&title->tiles[y][x], GRASS, (Vector2) { x * TILE_DRAW_SIZE, y * TILE_DRAW_SIZE }, false);
            }
        }
    }

    //init 2 sheep for title
    sheep_init(&title->sheep[0], 3, 4, false);
    sheep_init(&title->sheep[1], 5, 4, false);
    title->sheep_count = 2;

    title->z_animation = animation_create(450.0f, 3, 1 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    return true;
}

bool title_update(TitleScreen* title, GameTime game_time, const Input* input) {
    //update tiles
    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            tile_animation_update(title_get_tile(title, x, y), game_time);
        }
    }

    //update sheep
    for (int i = 0; i < title->sheep_count; i++) {
        Sheep *sheep = &title->sheep[i];
        sheep_update(sheep, game_time);
    }

    animation_update(&title->z_animation, game_time);

    if (input->undo) {
        return true;
    }
    return false;
}

ScreenTarget title_as_screen_target(TitleScreen *title) {
    return (ScreenTarget){ title, title_draw };
}

void title_draw(void *self) {
    TitleScreen *title = (TitleScreen *)self;
    
    //draw tiles
    for (int y = 0; y < SCREEN_TILE_SIZE; y++) {
        for (int x = 0; x < SCREEN_TILE_SIZE; x++) {
            tile_draw(title_get_tile(title, x, y));
        }
    }

    for (int i = 0; i < title->sheep_count; i++) {
        sheep_draw(&title->sheep[i]);
    }

    //draw lettering
    Rectangle s_source = (Rectangle) {0 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle h_source = (Rectangle) { 5 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle e_source = (Rectangle) { 4 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle p_source = (Rectangle) { 4 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    //Rectangle t_source = (Rectangle) { 0 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle r_source = (Rectangle) { 3 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle k_source = (Rectangle) { 1 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle x_source = (Rectangle) { 5 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle dog_source = (Rectangle) { 1 * TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

    Rectangle s_destination1 = (Rectangle) { 2 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle k_destination = (Rectangle) { 4 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    //sheep
    Rectangle s_destination2 = (Rectangle) { 2 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle h_destination = (Rectangle) { 3 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle e_destination1 = (Rectangle) { 4 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle e_destination2 = (Rectangle) { 5 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle p_destination = (Rectangle) { 6 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    //sleeping dog with zs above it
    Rectangle dog_destination = (Rectangle) { 7 * TILE_DRAW_SIZE, 5 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle z_destination = (Rectangle) { 7 * TILE_DRAW_SIZE, 4 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };

    //press x
    Rectangle p_destination2 = (Rectangle) { 2 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle r_destination = (Rectangle) { 3 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle e_destination3 = (Rectangle) { 4 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle s_destination3 = (Rectangle) { 5 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle s_destination4 = (Rectangle) { 6 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Rectangle x_destination = (Rectangle) { 8 * TILE_DRAW_SIZE, 8 * TILE_DRAW_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };

    Vector2 origin = (Vector2) { 0, 0 };

    DrawTexturePro(image_tileset, s_source, s_destination1, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, k_source, k_destination, origin, 0.0f, WHITE);

    DrawTexturePro(image_tileset, s_source, s_destination2, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, h_source, h_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, e_source, e_destination1, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, e_source, e_destination2, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, p_source, p_destination, origin, 0.0f, WHITE);

    DrawTexturePro(image_tileset, dog_source, dog_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, title->z_animation.source_rect, z_destination, origin, 0.0f, WHITE);

    DrawTexturePro(image_tileset, p_source, p_destination2, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, r_source, r_destination, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, e_source, e_destination3, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, s_source, s_destination3, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, s_source, s_destination4, origin, 0.0f, WHITE);
    DrawTexturePro(image_tileset, x_source, x_destination, origin, 0.0f, WHITE);
}