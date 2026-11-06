//include header files
#include "assets.h"
#include "constants.h"
#include "game_time.h"
#include "input.h"
#include "raylib.h"
#include "screen_canvas.h"
#include "world.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

//monogame defaults IsFixedTimeStep = true so we only move 1/60th of a second every frame
#define USE_FIXED_TIMESTEP 1
#define FIXED_DT (1.0f / 60.0f)

typedef struct Game {
    ScreenCanvas canvas;
    World world;
    GameTime game_time;
    Input input;
    float time_accumulator;
} Game;

static Game game;

static void gather_input(Input* input, const ScreenCanvas* canvas) {
    input->mouse_position = canvas_screen_to_virtual(canvas, GetMousePosition());
    input->move = (Vector2) {
        (float)((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))),
        (float)((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) - (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
    };
    input->click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    input->start |= IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    input->reset |= IsKeyPressed(KEY_R);
    input->undo |= IsKeyPressed(KEY_Z);
}

static void update_draw_frame(void) {
    //gather input each frame
    gather_input(&game.input, &game.canvas);

    //find delta time
    game.time_accumulator += GetFrameTime();
    if (game.time_accumulator > 0.25f) {
        game.time_accumulator = 0.25f;
    }

    while (game.time_accumulator >= FIXED_DT) {
        game.game_time.elapsed = FIXED_DT;
        game.game_time.total += FIXED_DT;
        world_update(&game.world, game.game_time, &game.input);

        //click is handled in one frame only, reset after
        game.input.click = game.input.start = game.input.reset = game.input.undo = false;
        game.time_accumulator -= FIXED_DT;
    }

    canvas_draw(&game.canvas, world_as_screen_target(&game.world));
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(VIRTUAL_WIDTH * WINDOW_SCALE, VIRTUAL_HEIGHT * WINDOW_SCALE, WINDOW_TITLE);
    InitAudioDevice();
    
    #if !defined(__EMSCRIPTEN__)
    //set directory so relative paths resolve against working directory
    ChangeDirectory(GetApplicationDirectory());
    #endif

    //load assets
    if (!assets_load()) {
        TraceLog(LOG_ERROR, "could not load assets/tileset.png");
    }
    //load and start playing background music
    Music background_music = LoadMusicStream("assets/audio/music/soko_sheep_background_music.ogg");
    SetMusicVolume(background_music, 0.7f);

    //initialization of game things
    canvas_init(&game.canvas, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, BLACK);
    if (!world_init(&game.world)) {
        TraceLog(LOG_ERROR, "could not load the first level");
    }

    //start playing music
    PlayMusicStream(background_music);

    #if defined(__EMSCRIPTEN__)
    //0 = browser refresh rate (requestAnimationFrame)
    emscripten_set_main_loop(update_draw_frame, 0, 1);
    #else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //draw and update
        update_draw_frame();
        //update stream buffer for music every frame
        UpdateMusicStream(background_music);
    }
    #endif

    world_unload(&game.world);
    canvas_unload(&game.canvas);
    assets_unload();
    //close audio device
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}