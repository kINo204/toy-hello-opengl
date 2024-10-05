#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_tex_coord;
out vec3 f_tex_coord;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 proj;

void main() {
	gl_Position = proj * view * model * vec4(v_pos, 1.0);
	f_tex_coord = v_tex_coord;
}