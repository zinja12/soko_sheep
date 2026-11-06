#include "assets.h"
#include "tile.h"

void tile_init(Tile *tile, TileType tile_type, Vector2 position, bool flip) {
    *tile = (Tile) { 0 };
    tile->x = (int)position.x;
    tile->y = (int)position.y;
    tile->flip = flip;
    tile->animated = get_animated(tile_type);
    tile->tile_type = tile_type;
    Vector2 source_position = get_tile_source_coordinates(tile_type);
    tile->source_x = (int)source_position.x;
    tile->source_y = (int)source_position.y;
    //convert to pixels on tile map for the source rect
    tile->source_rect = (Rectangle){
        (float)(source_position.x * TILE_SIZE),
        (float)(source_position.y * TILE_SIZE),
        (float)TILE_SIZE,
        (float)TILE_SIZE
    };

    if (tile->animated) {
        TileAnimationData tile_anim_data = get_tile_animation_data(tile_type);
        tile_init_animation(tile, tile->animated, tile_anim_data.frame_count, tile_anim_data.speed, tile_anim_data.source_x, tile_anim_data.source_y, TILE_SIZE);
    }
}

void tile_init_animation(Tile *tile, bool animated, int frame_count, float speed, int source_x, int source_y, int size) {
    if (!animated) {
        return;
    }
    tile->tile_animation = animation_create(speed, frame_count, source_x * size, source_y * size, size, size);
}

void tile_animation_update(Tile *tile, GameTime game_time) {
    if (tile->animated) {
        animation_update(&tile->tile_animation, game_time);
    }
}

void tile_draw(Tile *tile) {
    if (tile->tile_type == VOID) {
        return;
    }

    Vector2 center = (Vector2){
        tile->x + (TILE_DRAW_SIZE/2.0f),
        tile->y + (TILE_DRAW_SIZE/2.0f)
    };
    Rectangle destination = { center.x, center.y, TILE_DRAW_SIZE, TILE_DRAW_SIZE };
    Vector2 origin = (Vector2){ TILE_DRAW_SIZE / 2.0f, TILE_DRAW_SIZE / 2.0f };
    if (tile->animated) {
        Animation *animation = &tile->tile_animation;
        Rectangle animation_source = animation->source_rect;
        if (tile->flip) animation_source.width = -animation_source.width;

        DrawTexturePro(image_tileset, animation_source, destination, origin, 0.0f, WHITE);
    } else {
        Rectangle source = tile->source_rect;
        if (tile->flip) source.width = -source.width;
        DrawTexturePro(image_tileset, source, destination, origin, 0.0f, WHITE);
    }
}

Vector2 get_tile_source_coordinates(TileType tile_type) {
    switch (tile_type) {
        case GRASS:
            return (Vector2) { (float)GetRandomValue(0, 3), 1 };
        case WATER:
            return (Vector2) { 6, 4 };
        case TREE:
            return (Vector2) { 4, 1 };
        case PEN:
            return (Vector2) { 3, 7 };
        case FLOWER:
            return (Vector2) { 0, 7 };
        case WOLF:
            return (Vector2) { 6, 6 };
        case THISTLE:
            return (Vector2) { 6, 7 };
        case CHICKEN:
            return (Vector2) { 6, 3 };
        default:
            return (Vector2){ 0, 0 };
    }
}

bool get_animated(TileType tile_type) {
    switch (tile_type) {
        case WATER:
        case FLOWER:
        case WOLF:
        case THISTLE:
        case CHICKEN:
            return true;
        default:
            return false;
    }
}

TileAnimationData get_tile_animation_data(TileType tile_type) {
    Vector2 source = get_tile_source_coordinates(tile_type);
    TileAnimationData tile_anim_data = (TileAnimationData) { 0 };
    tile_anim_data.source_x = (int)source.x;
    tile_anim_data.source_y = (int)source.y;
    tile_anim_data.size = TILE_SIZE;
    tile_anim_data.speed = 400.0f;
    tile_anim_data.frame_count = (tile_type == FLOWER) ? 3 : 2;
    return tile_anim_data;
    
}

bool tile_blocks_dog(TileType tile_type) {
    switch (tile_type) {
        case VOID:
        case TREE:
        case WATER:
        case WOLF:
        case CHICKEN:
            return true;
        default:
            return false;
    }   
}

bool tile_blocks_sheep(TileType tile_type) {
    switch (tile_type) {
        case VOID:
        case TREE:
        case WOLF:
        case WATER:
        case CHICKEN:
            return true;
        default:
            return false;
    }
}