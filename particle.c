#include "assets.h"
#include "constants.h"
#include "particle.h"

#define PARTICLE_DRAW_SIZE TILE_DRAW_SIZE
#define PARTICLE_SPRITE_COUNT 3

#define PARTICLE_SPREAD 50

Vector2 PARTICLE_SPRITES[PARTICLE_SPRITE_COUNT] = { { 6, 2 }, { 7, 2 }, { 6, 1 } };

void particle_burst(ParticleSystem *particle_system, Vector2 position, Vector2 direction, int count, Color color) {
    for (int i = 0; i < count && particle_system->count < MAX_PARTICLES; i++) {
        float angle = GetRandomValue(0, 359) * DEG2RAD;
        if (direction.x != 0.0f || direction.y != 0.0f) {
            angle = atan2f(direction.y, direction.x) + GetRandomValue(-PARTICLE_SPREAD, PARTICLE_SPREAD) * DEG2RAD;
        }

        Particle *particle = &particle_system->particles[particle_system->count++];
        particle->position = position;
        particle->direction = (Vector2) { cosf(angle), sinf(angle) };
        particle->speed = (float)GetRandomValue(30, 70);
        particle->life_duration = GetRandomValue(25, 45) / 100.0f;
        particle->elapsed_life = 0.0f;
        particle->color = color;
    }
}

void particle_update(ParticleSystem *particle_system, GameTime game_time) {
    int i = 0;
    while (i < particle_system->count) {
        Particle *particle = &particle_system->particles[i];
        particle->elapsed_life += game_time.elapsed;

        //swap remove same as sheep so the last particle takes this slot
        if (particle->elapsed_life >= particle->life_duration) {
            particle_system->count--;
            *particle = particle_system->particles[particle_system->count];
            continue;
        }

        //slows over time
        float remaining = 1.0f - particle->elapsed_life / particle->life_duration;
        float distance = particle->speed * remaining * game_time.elapsed;
        particle->position.x += particle->direction.x * distance;
        particle->position.y += particle->direction.y * distance;
        i++;
    }
}

void particle_draw(ParticleSystem *particle_system) {
    for (int i = 0; i < particle_system->count; i++) {
        Particle *particle = &particle_system->particles[i];
        int sprite = (int)(particle->elapsed_life / particle->life_duration * PARTICLE_SPRITE_COUNT);
        Vector2 cell = PARTICLE_SPRITES[sprite];

        Rectangle source = { cell.x * TILE_SIZE, cell.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        Rectangle destination = {
            roundf(particle->position.x) - PARTICLE_DRAW_SIZE / 2,
            roundf(particle->position.y) - PARTICLE_DRAW_SIZE / 2,
            PARTICLE_DRAW_SIZE,
            PARTICLE_DRAW_SIZE
        };
        DrawTexturePro(image_tileset, source, destination, (Vector2){ 0, 0 }, 0.0f, particle->color);
    }
}