#pragma once

#include "gutils.h"

typedef struct _Camera {
    vec3 pos;
    vec3 dir;
    double yaw;
    double pitch;
    vec3 up;
} Camera;

extern float CAMERA_SPEED;
extern Camera camera;
extern int camera_cursor_is_first;

void camera_init();

void camera_update_view_trans(mat4x4 view);

void camera_move_forward();
void camera_move_backward();
void camera_move_leftward();
void camera_move_rightward();
void camera_move_upward();
void camera_move_downward();

void camera_cursor_cback(GLFWwindow* wnd, double x, double y);