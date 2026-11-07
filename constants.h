#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"

//small canvas (celeste size) that will be scaled up to the window
#define VIRTUAL_WIDTH 160
#define VIRTUAL_HEIGHT 160
#define WINDOW_SCALE 4
#define WINDOW_TITLE "sokosheep"

#define TILE_SIZE 8
#define TILE_DRAW_SIZE (TILE_SIZE * 2)

#define STEP_TIME 0.15f

#define LEVEL_COUNT 22
#define MAX_SHEEP 8

#define FADE_TIME 0.3f

#define BARK_TIME 0.4f

#define SCREEN_TILE_SIZE 10

//reference beepbox song
//https://www.beepbox.co/#9n31s6k0l00e0ft1qa7g0fj07r1i0o431T0v0u10f0qg01d04w2h0E0T0v0u00f0qg113d04w2h0E0T0v1u00f0qg3121d04w2h0E0T2v0u15f10w4qw02d03w0E0b03ch8zc08w0h8y8y4h4y4h4h4i8y8w0h000h8x408x4p22QAqqf7jnUJs4hUDDlOczPEaRdH-4qqfKSkjn_ikzHPc-h8WIiM6Y5XhUwm1yeyqIzB-218W9GOelTL8WXKOehTL8W9GOeZH_KUcnSZU9v8wx0BZL9x5U5dHUygAqZqKCE2Cz44382FB0IIzG5OIGKInbOOeEnaViKL0acIzG8WqWIKzb8Wyc0

#endif
