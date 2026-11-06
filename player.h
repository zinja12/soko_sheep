#ifndef PLAYER_H
#define PLAYER_H

#include "animation.h"
#include "raylib.h"
#include "game_time.h"
#include "tile.h"

typedef struct Player {
    //tile the player occupies, set to the destination as soon as a step starts
    //tile coords for player
    //these are set to the destination as soon as the player step starts
    int tile_x, tile_y;
    //pixel positions for player for lerping between tiles
    Vector2 from_position, to_position, position, last_direction;
    float move_time, move_duration;
    bool moving, arrived;
    bool barking;
    float bark_timer;

    //right hand rule from north up top (0-4)
    int current_frame;
    bool flip;
    Rectangle walk_source;
} Player;

void player_init(Player *player, int tile_x, int tile_y);
void player_start_move(Player *player, int tile_x, int tile_y, float duration, int dx, int dy);
void player_update(Player *player, GameTime game_time);
Vector2 player_get_anim_source(Player *player, int dx, int dy);
void player_set_direction(Player *player, int dx, int dy);
void player_face_direction(Player *player, int dx, int dy);
void player_draw(const Player *player);
void bark_draw(Vector2 position, Vector2 direction);

#endif