#include "../include/camera.h"

Camera camera;

float CAMERA_SPEED = 0.f;

void camera_init() {
    // default pos vector
    camera.pos[0] = 0.0f;
    camera.pos[1] = 0.0f;
    camera.pos[2] = 3.0f;

    // default dir vector
    camera.dir[0] = 0.0f;
    camera.dir[1] = 0.0f;
    camera.dir[2] = -1.f;
    
    // default up vector
    camera.up[0] = 0.0f;
    camera.up[1] = 1.0f;
    camera.up[2] = 0.0f;
}

void camera_update_view_trans(mat4x4 view) {
    vec3 center;
    vec3_add(center, camera.pos, camera.dir);
    mat4x4_look_at(view, camera.pos, center, camera.up);
}

// This is my supplementary function!
static inline void vec3_mul_scalar(vec3 r, vec3 const v, float f) {
    for (int i = 0; i < 3; i++) { r[i] = v[i] * f; }
}

void camera_move_forward() {
    vec3 move;
    vec3_mul_scalar(move, camera.dir, CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
}

void camera_move_backward() {
    vec3 move;
    vec3_mul_scalar(move, camera.dir, 0.f - CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
}

void camera_move_leftward() {
    vec3 left;
    vec3_mul_cross(left, camera.up, camera.dir);
    vec3_norm(left, left);
    vec3 move;
    vec3_mul_scalar(move, left, CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
}

void camera_move_rightward() {
    vec3 right;
    vec3_mul_cross(right, camera.dir, camera.up);
    vec3_norm(right, right);
    vec3 move;
    vec3_mul_scalar(move, right, CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
}