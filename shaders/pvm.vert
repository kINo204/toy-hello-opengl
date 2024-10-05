#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 proj;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);
}