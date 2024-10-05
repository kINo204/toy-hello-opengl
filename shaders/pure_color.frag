#version 330 core

in  vec3 f_tex_coord;
out vec4 fragColor;

void main() {
    fragColor = vec4(0.9412f, 0.5255f, 0.3137f, 1.0f);
}