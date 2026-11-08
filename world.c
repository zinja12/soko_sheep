#include "assets.h"
#include "constants.h"
#include "game_time.h"
#include "input.h"
#include "raymath.h"
#include "screen_canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"

static const int DIRECTION_X[4] = { 0, 1, 0, -1 };
static const int DIRECTION_Y[4] = { -1, 0, 1, 0 };

#define HIT_PARTICLES 3
#define PEN_PARTICLES 6

static inline Tile *world_get_tile(World *world, int x, int y) {
    if (x < 0 || y < 0 || x >= world->width || y >= world->height) {
        return VOID;
    }
    return &world->tiles[y * world->width + x];
}

//anything off map counts as VOID
static TileType world_tile_type(World *world, int x, int y) {
    if (x < 0 || y < 0 || x >= world->width || y >= world->height) {
        return VOID;
    }
    return world_get_tile(world, x, y)->tile_type;
}

static TileType tile_type_from_char(char c) {
    switch (c) {
        case 'S':
        case 'R':
        case 'D':
        case 'g':
            return GRASS;
        case '~':
            return WATER;
        case '#':
            return PEN;
        case 't':
            return TREE;
        case 'f':
            return FLOWER;
        case 'w':
            return WOLF;
        case 'h':
            return THISTLE;
        case 'c':
            return CHICKEN;
        default:
            return VOID;
    }
}

bool world_load(World *world, const char *path) {
    char *text = LoadFileText(path);
    //failed somewhere
    if (text == NULL) {
        return false;
    }

    //first pass: measure the map so we know how much to allocate
    int width = 0, height = 0, x = 0;
    for (char *c = text; *c != '\0'; c++) {
        //files saved on windows end lines with \r\n
        if (*c == '\r') continue;
        if (*c == '\n') {
            height++;
            x = 0;
            continue;
        }
        x++;
        if (x > width) width = x;
    }
    //last line may not end with a newline
    if (x > 0) height++;

    if (width == 0 || height == 0) {
        UnloadFileText(text);
        return false;
    }

    //calloc zeroes memory, so cells past the end of a short line stay VOID
    Tile *tiles = calloc((size_t)width * height, sizeof(Tile));
    if (tiles == NULL) {
        UnloadFileText(text);
        return false;
    }

    //second pass: create the tiles and place the dog and sheep
    x = 0;
    int y = 0;
    int spawn_x = -1, spawn_y = -1, sheep_count = 0;
    Sheep sheep[MAX_SHEEP];
    for (char *c = text; *c != '\0'; c++) {
        //windows file end lines
        if (*c == '\r') continue;
        if (*c == '\n') {
            x = 0;
            y++;
            continue;
        }
        //init dog
        if (*c == 'D') {
            spawn_x = x;
            spawn_y = y;
        } else if ((*c == 'S' || *c == 'R') && sheep_count < MAX_SHEEP) {
            //init sheep
            sheep_init(&sheep[sheep_count++], x, y, *c == 'R');
        }
        //calculate position
        Vector2 position = { (float)(x * TILE_DRAW_SIZE), (float)(y * TILE_DRAW_SIZE) };
        //init tile
        tile_init(&tiles[y * width + x], tile_type_from_char(*c), position, false);
        x++;
    }
    UnloadFileText(text);

    //level needs a dog, and at least one sheep or it would count as solved instantly
    if (spawn_x < 0 || sheep_count == 0) {
        free(tiles);
        return false;
    }

    //swap in the new map only once it loaded, so a broken file on reload keeps the old one
    free(world->tiles);
    world->tiles = tiles;
    world->width = width;
    world->height = height;
    memcpy(world->sheep, sheep, sizeof(Sheep) * sheep_count);
    world->sheep_count = sheep_count;
    player_init(&world->player, spawn_x, spawn_y);
    world->chicken_bark_time = 0.0f;
    world->particle_system.count = 0;
    world->shake = (ShakeEffect){ 0 };
    world->undo_next = world->undo_count = 0;
    return true;
}

bool world_load_level(World *world, int level_idx) {
    char path[64];
    snprintf(path, sizeof(path), "assets/maps/level%d.txt", level_idx + 1);
    if (!world_load(world, path)) {
        return false;
    }
    world->level_idx = level_idx;
    return true;
}

void world_unload(World *world) {
    free(world->tiles);
    world->tiles = NULL;
    world->width = world->height = 0;
}

bool world_init(World* world) {
    *world = (World){ 0 };
    return world_load_level(world, 0);
}

void world_start_transition(World *world, int next_level) {
    world->next_level = next_level;
    world->transition_time = 0.0f;
    world->transitioning = true;
}

Sheep *world_sheep_at(World *world, int x, int y) {
    //find sheep
    for (int i = 0; i < world->sheep_count; i++) {
        if (world->sheep[i].tile_x == x && world->sheep[i].tile_y == y) {
            return &world->sheep[i];
        }
    }
    return NULL;
}

bool world_sheep_can_enter(World *world, int x, int y) {
    if (tile_blocks_sheep(world_tile_type(world, x, y))) {
        return false;
    }
    if (world->player.tile_x == x && world->player.tile_y == y) {
        return false;
    }
    return world_sheep_at(world, x, y) == NULL;
}

//in air sheep can cross water, only if it can land on the tile after though
bool world_sheep_can_jump(World *world, Sheep *sheep) {
    int x = sheep->tile_x + sheep->dx;
    int y = sheep->tile_y + sheep->dy;
    if (world_tile_type(world, x, y) == WATER) {
        return world_sheep_can_enter(world, x + sheep->dx, y + sheep->dy);
    }
    return world_sheep_can_enter(world, x, y);
}

void world_sheep_arrived(World *world, Sheep *sheep) {
    TileType tile_type = world_tile_type(world, sheep->tile_x, sheep->tile_y);

    if (tile_type == PEN) {
        PlaySound(sound_effects[SFX_PEN]);
        Vector2 center = { sheep->position.x + TILE_SIZE, sheep->position.y + TILE_SIZE };
        particle_burst(&world->particle_system, center, (Vector2) { 0, 0 }, PEN_PARTICLES, WHITE);

        world->sheep_count--;
        *sheep = world->sheep[world->sheep_count];
        return;
    }

    bool can_continue = world_sheep_can_enter(world, sheep->tile_x + sheep->dx, sheep->tile_y + sheep->dy);

    //tile collision
    if (sheep->in_air) {
        //passing over this tile, so it has no effect
        sheep->in_air = false;
        if (can_continue) {
            sheep_start_step(sheep, sheep->dx, sheep->dy, STEP_TIME);
            return;
        }
    } else if (tile_type == THISTLE && sheep->state == SHEEP_PUSHED) {
        //bucked one more tile, water stops this like any other blocker
        sheep->state = SHEEP_REST;
        if (can_continue) {
            sheep_start_step(sheep, sheep->dx, sheep->dy, STEP_TIME);
            return;
        }
    } else if (tile_type == THISTLE && sheep->state == SHEEP_SLIDE && world_sheep_can_jump(world, sheep)) {
        sheep->in_air = true;
        sheep_start_step(sheep, sheep->dx, sheep->dy, STEP_TIME);
        return;
    }

    //flowers: the conveyor and the emergency brake are the same check
    for (int i = 0; i < 4; i++) {
        int x = sheep->tile_x + DIRECTION_X[i];
        int y = sheep->tile_y + DIRECTION_Y[i];
        if (world_tile_type(world, x, y) == FLOWER && world_sheep_can_enter(world, x, y)) {
            Tile *flower = world_get_tile(world, x, y);
            tile_init(flower, GRASS, (Vector2){ (float)flower->x, (float)flower->y }, false);
            sheep->state = SHEEP_REST;
            sheep_start_step(sheep, DIRECTION_X[i], DIRECTION_Y[i], STEP_TIME);
            return;
        }
    }

    //wolves interrupt slides
    bool near_wolf = false;
    for (int i = 0; i < 4; i++) {
        int x = sheep->tile_x + DIRECTION_X[i];
        int y = sheep->tile_y + DIRECTION_Y[i];
        if (world_tile_type(world, x, y) != WOLF) {
            continue;
        }
        near_wolf = true;

        int away_x = -DIRECTION_X[i];
        int away_y = -DIRECTION_Y[i];
        if (world_sheep_can_enter(world, sheep->tile_x + away_x, sheep->tile_y + away_y)) {
            sheep->state = SHEEP_REST;
            PlaySound(sound_effects[SFX_WOLF_GROWL]);
            sheep_start_step(sheep, away_x, away_y, STEP_TIME);
            return;
        }
    }

    bool still_sliding = sheep->state == SHEEP_SLIDE && !near_wolf;
    sheep->state = SHEEP_REST;
    if (still_sliding) {
        //check if the knock moves the sheep in front
        bool hit_sheep = world_sheep_at(world, sheep->tile_x + sheep->dx, sheep->tile_y + sheep->dy) != NULL;
        bool knocked = world_sheep_knock(world, sheep, sheep->dx, sheep->dy);
        if (!knocked) {
            //nothing moves so sheep must have hit something
            Vector2 contact = { sheep->position.x + TILE_SIZE + sheep->dx * TILE_SIZE, sheep->position.y + TILE_SIZE + sheep->dy * TILE_SIZE };
            particle_burst(&world->particle_system, contact, (Vector2){ (float)-sheep->dx, (float)-sheep->dy }, HIT_PARTICLES, WHITE);
        }
        if (hit_sheep || !knocked) {
            PlaySound(sound_effects[SFX_SHEEP_HIT]);
            shake_trigger(&world->shake, HIT_SHAKE_STRENGTH, HIT_SHAKE_TIME);
        }
    }
}

void world_player_arrived(World *world, Player *player) {
    for (int i = 0; i < 4; i++) {
        int x = player->tile_x + DIRECTION_X[i];
        int y = player->tile_y + DIRECTION_Y[i];
        if (world_tile_type(world, x, y) != CHICKEN) {
            continue;
        }
        
        int away_x = -DIRECTION_X[i];
        int away_y = -DIRECTION_Y[i];
        int target_x = player->tile_x + away_x;
        int target_y = player->tile_y + away_y;
        if (!tile_blocks_dog(world_tile_type(world, target_x, target_y)) && world_sheep_at(world, target_x, target_y) == NULL) {
            world->chicken_bark_time = BARK_TIME;
            world->chicken_bark_position = tile_to_position(x, y);
            PlaySound(sound_effects[SFX_CHICK_CHIRP]);
            player_start_move(player, target_x, target_y, STEP_TIME, away_x, away_y);
            return;
        }
    }
}

bool world_sheep_knock(World *world, Sheep *sheep, int dx, int dy) {
    Sheep *next_sheep = world_sheep_at(world, sheep->tile_x + dx, sheep->tile_y + dy);
    while (next_sheep != NULL) {
        sheep = next_sheep;
        next_sheep = world_sheep_at(world, sheep->tile_x + dx, sheep->tile_y + dy);
    }
    
    if (world_sheep_can_enter(world, sheep->tile_x + dx, sheep->tile_y + dy)) {
        sheep->state = SHEEP_SLIDE;
        //PlaySound(sound_effects[SFX_SHEEP_BLEAT]);
        sheep_start_step(sheep, dx, dy, STEP_TIME);
        return true;
    }
    return false;
}

void world_bark(World *world) {
    int dx = (int)world->player.last_direction.x;
    int dy = (int)world->player.last_direction.y;
    int x = world->player.tile_x + dx;
    int y = world->player.tile_y + dy;

    //set bark on player for visual
    world->player.barking = true;
    //play sfx
    PlaySound(sound_effects[SFX_DOG_BARK]);

    //trees and the edge of the map block the bark
    while (world_tile_type(world, x, y) != TREE && world_tile_type(world, x, y) != VOID) {
        Sheep *sheep = world_sheep_at(world, x, y);
        if (sheep != NULL) {
            if (world_sheep_knock(world, sheep, dx, dy)) {
                PlaySound(sound_effects[SFX_SHEEP_BLEAT]);
            }
            return;
        }
        x += dx;
        y += dy;
    }
}

void world_move_player(World *world, int dx, int dy) {
    int target_x = world->player.tile_x + dx;
    int target_y = world->player.tile_y + dy;

    Sheep *sheep = world_sheep_at(world, target_x, target_y);
    if (sheep != NULL) {
        if (sheep->is_ram || !world_sheep_can_enter(world, target_x + dx, target_y + dy)) {
            player_face_direction(&world->player, dx, dy);
            return;
        }
        sheep->state = SHEEP_PUSHED;
        sheep_start_step(sheep, dx, dy, STEP_TIME);
    } else if (tile_blocks_dog(world_tile_type(world, target_x, target_y))) {
        player_face_direction(&world->player, dx, dy);
        return;
    }

    player_start_move(&world->player, target_x, target_y, STEP_TIME, dx, dy);
}

bool world_is_solved(World *world) {
    for (int i = 0; i < world->sheep_count; i++) {
        if (world_tile_type(world, world->sheep[i].tile_x, world->sheep[i].tile_y) != PEN) {
            return false;
        }
    }
    return true;
}

bool world_is_transacting(World *world) {
    if (world->player.moving) {
        return true;
    }
    for (int i = 0; i < world->sheep_count; i++) {
        if (world->sheep[i].moving) {
            return true;
        }
    }
    return false;
}

void world_undo_step_save(World *world) {
    UndoStep *undo_step = &world->undo_steps[world->undo_next];
    undo_step->player = world->player;
    memcpy(undo_step->sheep, world->sheep, sizeof(Sheep) * world->sheep_count);
    undo_step->sheep_count = world->sheep_count;
    for (int i = 0; i < world->width * world->height && i < UNDO_MAX_TILES; i++) {
        undo_step->tile_types[i] = world->tiles[i].tile_type;
    }
}

void world_undo_step_commit_transaction(World *world) {
    if (!world_is_transacting(world)) {
        return;
    }
    world->undo_next = (world->undo_next + 1) % UNDO_MAX;
    if (world->undo_count < UNDO_MAX) {
        world->undo_count++;
    }
}

void world_undo(World *world) {
    if (world->undo_count == 0) {
        return;
    }
    world->undo_next = (world->undo_next + UNDO_MAX - 1) % UNDO_MAX;
    world->undo_count--;

    UndoStep *step = &world->undo_steps[world->undo_next];
    world->player = step->player;
    world->player.barking = false;
    world->player.bark_timer = 0.0f;
    world->chicken_bark_time = 0.0f;
    memcpy(world->sheep, step->sheep, sizeof(Sheep) * step->sheep_count);
    world->sheep_count = step->sheep_count;
    for (int i = 0; i < world->width * world->height && i < UNDO_MAX_TILES; i++) {
        Tile *tile = &world->tiles[i];
        if (tile->tile_type != step->tile_types[i]) {
            tile_init(tile, step->tile_types[i], (Vector2){ (float)tile->x, (float)tile->y }, false);
        }
    }
}

void world_update(World* world, GameTime game_time, const Input* input) {
    //update tiles
    for (int y = 0; y < world->height; y++) {
        for (int x = 0; x < world->width; x++) {
            tile_animation_update(world_get_tile(world, x, y), game_time);
        }
    }

    if (world->chicken_bark_time > 0.0f) {
        world->chicken_bark_time -= game_time.elapsed;
    }

    particle_update(&world->particle_system, game_time);

    shake_update(&world->shake, game_time);

    if (world->transitioning) {
        bool was_fading_out = world->transition_time < FADE_TIME;
        world->transition_time += game_time.elapsed;
        //swap levels while the screen is fully black
        if (was_fading_out && world->transition_time >= FADE_TIME) {
            if (world->next_level >= LEVEL_COUNT) {
                world->finished = true;
                return;
            }
            world_load_level(world, world->next_level);
        }
        if (world->transition_time >= FADE_TIME * 2.0f) {
            world->transitioning = false;
        }
        return;
    }

    if (input->reset) {
        world_load_level(world, world->level_idx);
        return;
    }

    player_update(&world->player, game_time);
    if (world->player.arrived) {
        world->player.arrived = false;
        world_player_arrived(world, &world->player);
    }

    for (int i = 0; i < world->sheep_count; i++) {
        Sheep *sheep = &world->sheep[i];
        if (sheep_update(sheep, game_time)) {
            world_sheep_arrived(world, sheep);
        }
    }

    // bool sheep_moving = false;
    // for (int i = 0; i < world->sheep_count; i++) {
    //     if (world->sheep[i].moving) {
    //         sheep_moving = true;
    //     }
    // }

    //input stays locked until the whole world reaction has finished
    if (world_is_transacting(world)) {
        return;
    }

    if (world_is_solved(world)) {
        world_start_transition(world, world->level_idx + 1);
        return;
    }

    if (input->undo) {
        world_undo(world);
        return;
    }

    if (input->start && !world->player.barking) {
        world_undo_step_save(world);
        world_bark(world);
        world_undo_step_commit_transaction(world);
        return;
    }

    //one axis at a time, horizontal wins if both are held
    int dx = (int)input->move.x;
    int dy = (dx != 0) ? 0 : (int)input->move.y;
    if (dx != 0 || dy != 0) {
        world_undo_step_save(world);
        world_move_player(world, dx, dy);
        world_undo_step_commit_transaction(world);
    }
}

ScreenTarget world_as_screen_target(World *world) {
    return (ScreenTarget){ world, world_draw };
}

void world_draw(void *self) {
    World *world = (World *)self;
    
    BeginMode2D(shake_get_camera(&world->shake));

    //draw tiles
    for (int y = 0; y < world->height; y++) {
        for (int x = 0; x < world->width; x++) {
            tile_draw(world_get_tile(world, x, y));
        }
    }

    for (int i = 0; i < world->sheep_count; i++) {
        sheep_draw(&world->sheep[i]);
    }

    player_draw(&world->player);
    
    //draw chicken bark
    if (world->chicken_bark_time > 0.0f) {
        bark_draw(world->chicken_bark_position, (Vector2) { 1, 0 });
    }

    particle_draw(&world->particle_system);

    EndMode2D();

    if (world->transitioning) {
        //0 to 1 over the fade out, back to 0 over the fade in
        float alpha = 1.0f - fabsf(world->transition_time / FADE_TIME - 1.0f);
        DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, Fade(BLACK, alpha));
    }
}