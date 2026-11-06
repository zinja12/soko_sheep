#ifndef PARTICLE_H
#define PARTICLE_H

#include "game_time.h"
#include "raylib.h"

#define MAX_PARTICLES 100

typedef struct Particle {
    Vector2 position;
    Vector2 direction;
    float speed;
    float life_duration, elapsed_life;
    Color color;
} Particle;

typedef struct ParticleSystem {
    Particle particles[MAX_PARTICLES];
    int count;
} ParticleSystem;

void particle_burst(ParticleSystem *particle_system, Vector2 position, Vector2 direction, int count, Color color);
void particle_update(ParticleSystem *particle_system, GameTime game_time);
void particle_draw(ParticleSystem *particle_system);

#endif