#version 330 core

in  vec3 our_color;

out vec4 fragColor;

void main() {
    fragColor = vec4(our_color, 1.0);
}