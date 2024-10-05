#pragma once

#include "gutils.h"

typedef struct _Camera {
    vec3 pos;
    vec3 dir;
    vec3 up;
} Camera;

extern float CAMERA_SPEED;

extern Camera camera;

void camera_init();

void camera_update_view_trans(mat4x4 view);

void camera_move_forward();
void camera_move_backward();
void camera_move_leftward();
void camera_move_rightward();
