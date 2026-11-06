#include "assets.h"
#include "rlgl.h"
#include <stddef.h>

//define global sound effect paths
Sound sound_effects[SFX_COUNT] = { 0 };
char *SFX_PATHS[SFX_COUNT] = {
    [SFX_DOG_BARK] = "assets/audio/sfx/dog_bark1_gameboy.ogg",
    [SFX_SHEEP_BLEAT] = "assets/audio/sfx/sheep_bleat_gameboy.ogg",
    [SFX_CHICK_CHIRP] = "assets/audio/sfx/chick_chirp_gameboy.ogg",
    [SFX_WOLF_GROWL] = "assets/audio/sfx/wolf_growl_gameboy.ogg",
    [SFX_PEN] = "assets/audio/sfx/sheep_pen_gameboy.ogg"
};
//define global tileset
Texture2D image_tileset = { 0 };

bool assets_load(void) {
    //paths are relative to working directory
    //load audio
    for (int i = 0; i < SFX_COUNT; i++) {
        sound_effects[i] = LoadSound(SFX_PATHS[i]);
        if (!IsSoundValid(sound_effects[i])) {
            TraceLog(LOG_ERROR, "could not load %s", SFX_PATHS[i]);
        }
        SetSoundVolume(sound_effects[i], 0.8f);
    }

    //load image tileset
    image_tileset = LoadTexture("assets/tileset.png");

    //MonoGame's SamplerState.PointClamp: no smoothing when scaled, so pixel art stays crisp and
    //in raylib the filter is a property of each texture rather than of the draw call
    SetTextureFilter(image_tileset, TEXTURE_FILTER_POINT);

    return IsTextureValid(image_tileset);
}

void assets_unload(void) {
    //unload the sound effects
    for (int i = 0; i < SFX_COUNT; i++) {
        UnloadSound(sound_effects[i]);
    }
    //unload the texture
    UnloadTexture(image_tileset);
}