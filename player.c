#include "assets.h"
#include "constants.h"
#include "player.h"
#include "raymath.h"

#define PLAYER_WALK_FRAME_COUNT 2

#define PLAYER_WALK_Y 6
#define PLAYER_WALK_SIDE_X 1
#define PLAYER_WALK_UP_X 4
#define PLAYER_WALK_DOWN_X 2
#define PLAYER_WALK_DOWN_Y 5

static Vector2 tile_to_position(int tile_x, int tile_y) {
    return (Vector2){ (float)(tile_x * TILE_DRAW_SIZE), (float)(tile_y * TILE_DRAW_SIZE) };
}

void player_init(Player *player, int tile_x, int tile_y) {
    *player = (Player){ 0 };
    player->tile_x = tile_x;
    player->tile_y = tile_y;
    player->position = tile_to_position(tile_x, tile_y);
    player->from_position = player->to_position = player->position;
    //player animation
    player->current_frame = 0;
    player->walk_source = (Rectangle){ PLAYER_WALK_SIDE_X * TILE_SIZE, PLAYER_WALK_Y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    
    //set default last direction
    player->last_direction = (Vector2) { 1, 0 };
}

void player_set_direction(Player *player, int dx, int dy) {
    Vector2 source_coords = player_get_anim_source(player, dx, dy);
    player->walk_source = (Rectangle) { source_coords.x * TILE_SIZE, source_coords.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    player->last_direction = (Vector2) { dx, dy };
}

//turn while blocked
void player_face_direction(Player *player, int dx, int dy) {
    if (player->last_direction.x == dx && player->last_direction.y == dy) {
        return;
    }
    //set direction player is facing
    player_set_direction(player, dx, dy);
}

void player_start_move(Player *player, int tile_x, int tile_y, float duration, int dx, int dy) {
    player->from_position = player->position;
    player->to_position = tile_to_position(tile_x, tile_y);
    player->tile_x = tile_x;
    player->tile_y = tile_y;
    player->move_time = 0.0f;
    player->move_duration = duration;
    player->moving = true;
    //set player direction
   player_set_direction(player, dx, dy);
}

void player_update(Player *player, GameTime game_time) {
    //barking visual
    if (player->barking) {
        player->bark_timer += game_time.elapsed;
        if (player->bark_timer >= BARK_TIME) {
            player->bark_timer = 0.0f;
            player->barking = false;
        }
    }
    
    if (!player->moving) {
        return;
    }

    player->move_time += game_time.elapsed;
    float t = fminf(player->move_time / player->move_duration, 1.0f);
    Vector2 position = Vector2Lerp(player->from_position, player->to_position, t);
    //whole pixels only, same as the camera, so the sprite doesn't shimmer
    player->position = (Vector2){ roundf(position.x), roundf(position.y) };

    if (t >= 1.0f) {
        player->position = player->to_position;
        player->moving = false;
        player->arrived = true;
    }
}

Vector2 player_get_anim_source(Player *player, int dx, int dy) {
    bool was_flipped = player->flip;
    int source_x = PLAYER_WALK_SIDE_X;
    int source_y = PLAYER_WALK_Y;
    if (dx > 0) {
        //right
        player->flip = false;
    } else if (dx < 0) {
        //left
        player->flip = true;
    } else if (dy < 0) {
        //up
        player->flip = false;
        source_x = PLAYER_WALK_UP_X;
    } else if (dy > 0) {
        //down
        player->flip = false;
        source_x = PLAYER_WALK_DOWN_X;
        source_y = PLAYER_WALK_DOWN_Y;
    }

    //left and right show up as a flip change, up and down as a dy change
    bool turned_around = (player->flip != was_flipped) || (dy != (int)player->last_direction.y);
    if (turned_around) {
        player->current_frame = 0;
    } else {
        player->current_frame = (player->current_frame + 1) % PLAYER_WALK_FRAME_COUNT;
    }
    return (Vector2) { source_x + player->current_frame, source_y };
}

void player_draw(const Player *player) {
    Rectangle source = player->walk_source;
    if (player->flip) source.width = -source.width;
    Rectangle destination = (Rectangle) { player->position.x, player->position.y, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    DrawTexturePro(image_tileset, source, destination, (Vector2){ 0, 0 }, 0.0f, WHITE);

    if (player->barking) {
        bark_draw(player->position, player->last_direction);
    }
}

void bark_draw(Vector2 position, Vector2 direction) {
    Rectangle bark_source = (Rectangle) { 3 * TILE_SIZE, 6 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle bark_destination = (Rectangle) { position.x + direction.x * TILE_DRAW_SIZE + TILE_SIZE, position.y + direction.y * TILE_DRAW_SIZE + TILE_SIZE, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    float rotation = atan2f(direction.y, direction.x) * RAD2DEG;
    DrawTexturePro(image_tileset, bark_source, bark_destination, (Vector2){ TILE_SIZE, TILE_SIZE }, rotation, WHITE);
}