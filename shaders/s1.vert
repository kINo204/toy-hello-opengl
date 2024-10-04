#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;

out vec3 our_color;
out vec2 our_tex_coord;

uniform mat4x4 trans_mat;

void main() {
	gl_Position = trans_mat * vec4(pos, 1.0);
	our_color = color;
	our_tex_coord = tex_coord;
}