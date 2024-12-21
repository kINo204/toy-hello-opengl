#version 330 core
layout (location = 0) in vec3 a_pos;

uniform mat4x4 model;
uniform mat4x4 view_light_space;
uniform mat4x4 proj;

void main() {
    gl_Position = proj * view_light_space * model * vec4(a_pos, 1.0);
}