#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

typedef enum SoundEffects {
    SFX_DOG_BARK,
    SFX_SHEEP_BLEAT,
    SFX_CHICK_CHIRP,
    SFX_WOLF_GROWL,
    SFX_PEN,
    SFX_COUNT
} SoundEffects;

extern Sound sound_effects[SFX_COUNT];
extern Texture2D image_tileset;

//returns false if a file is missing
bool assets_load(void);
//unload assets
void assets_unload(void);

#endif