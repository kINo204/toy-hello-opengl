#include "../include/camera.h"
#include <math.h>

Camera camera;

float CAMERA_SPEED = 0.f; // moving speed of the camera.

void camera_init() {
    // default pos vector
    camera.pos[0] = 0.0f;
    camera.pos[1] = 0.0f;
    camera.pos[2] = 3.0f;

    // default dir vector
    camera.dir[0] = 0.0f;
    camera.dir[1] = 0.0f;
    camera.dir[2] = -1.f;
    camera.yaw = -1.5708; // -90 deg
    camera.pitch = 0;
    
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

void camera_move_upward() {
    vec3 move;
    vec3_mul_scalar(move, camera.up, CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
}

void camera_move_downward() {
    vec3 move;
    vec3_mul_scalar(move, camera.up, 0.f - CAMERA_SPEED);
    vec3_add(camera.pos, camera.pos, move);
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

const double MOUSE_SENSITIVITY = 0.0008;

int camera_cursor_is_first = 1;
void camera_cursor_cback(GLFWwindow* wnd, double x, double y) {
    static double last_x, last_y;
    if (camera_cursor_is_first) {
        last_x = x;
        last_y = y;
        camera_cursor_is_first = 0;
    }
    double delta_x = x - last_x;
    double delta_y = y - last_y;
    last_x = x;
    last_y = y;

    camera.yaw += MOUSE_SENSITIVITY * delta_x;
    camera.pitch -= MOUSE_SENSITIVITY * delta_y;

    // Lock pitch in range.
    camera.pitch = camera.pitch > 1.57 ? 1.57 :
                    camera.pitch < -1.57 ? -1.57 :
                    camera.pitch;

    camera.dir[0] = cos(camera.yaw) * cos(camera.pitch);
    camera.dir[1] = sin(camera.pitch);
    camera.dir[2] = sin(camera.yaw) * cos(camera.pitch);
    vec3_norm(camera.dir, camera.dir);
}