#ifndef SCREEN_TARGET_H
#define SCREEN_TARGET_H

typedef struct ScreenTarget {
    //object of the target
    void *self;
    //draw method (abstracted via function pointer)
    void (*draw)(void *self);
} ScreenTarget;

#endif