#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 view_light_space[2];
uniform mat4x4 proj;

out vec3 norm;
out vec3 frag_pos;
out vec4 frag_pos_light_space[2];

void main() {
	gl_Position = proj * view * model * vec4(a_pos, 1.0);
	norm = a_norm;
	frag_pos = vec3(model * vec4(a_pos, 1.0));
	frag_pos_light_space[0] = proj * view_light_space[0] * model * vec4(a_pos, 1.0);
	frag_pos_light_space[1] = proj * view_light_space[1] * model * vec4(a_pos, 1.0);
}