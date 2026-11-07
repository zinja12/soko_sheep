#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "game_time.h"
#include "input.h"
#include "particle.h"
#include "player.h"
#include "screen_target.h"
#include "sheep.h"
#include "tile.h"

#define UNDO_MAX 256
#define UNDO_MAX_TILES ((VIRTUAL_WIDTH / TILE_DRAW_SIZE) * (VIRTUAL_HEIGHT / TILE_DRAW_SIZE))

typedef struct UndoStep {
    Player player;
    Sheep sheep[MAX_SHEEP];
    int sheep_count;
    TileType tile_types[UNDO_MAX_TILES];
} UndoStep;

typedef struct World {
    Tile *tiles;
    int width, height;
    
    Player player;
    Sheep sheep[MAX_SHEEP];
    int sheep_count;

    int level_idx, next_level;
    bool transitioning;
    float transition_time;
    
    float chicken_bark_time;
    Vector2 chicken_bark_position;

    ParticleSystem particle_system;

    bool finished;

    UndoStep undo_steps[UNDO_MAX];
    int undo_next, undo_count;
} World;

bool world_init(World* world);
bool world_load(World *world, const char *path);
bool world_load_level(World *world, int level_idx);
void world_unload(World *world);
void world_update(World* world, GameTime game_time, const Input* input);
void world_draw(void *self);
ScreenTarget world_as_screen_target(World *world);

void world_start_transition(World *world, int next_level);
Sheep *world_sheep_at(World *world, int x, int y);
bool world_sheep_can_enter(World *world, int x, int y);
bool world_sheep_can_jump(World *world, Sheep *sheep);
void world_sheep_arrived(World *world, Sheep *sheep);
void world_player_arrived(World *world, Player *player);
bool world_sheep_knock(World *world, Sheep *sheep, int dx, int dy);
void world_bark(World *world);
void world_move_player(World *world, int dx, int dy);
bool world_is_solved(World *world);

bool world_is_transacting(World *world);
//call before move
void world_undo_step_save(World *world);
//call after move
void world_undo_step_commit_transaction(World *world);
void world_undo(World *world);

#endif