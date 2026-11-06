#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"

//define input struct
typedef struct Input {
    //mouse position
    Vector2 mouse_position;
    //held WASD / arrow keys
    //uses -1, 0, 1 per axis
    //note: held keys are safe to re-read
    Vector2 move;
    bool click;
    //space or enter pressed this frame
    bool start;
    //R pressed thie frame
    bool reset;
    //Z undo
    bool undo;
} Input;

#endif