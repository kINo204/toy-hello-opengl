#version 330 core

in  vec2 f_tex_coord;
out vec4 fragColor;

uniform sampler2D tex;

void main() {
    fragColor = texture(tex, f_tex_coord);
}