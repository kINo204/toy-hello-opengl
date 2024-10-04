#version 330 core

in  vec3 our_color;
in  vec2 our_tex_coord;

out vec4 fragColor;

uniform sampler2D our_tex;
uniform sampler2D our_tex1;

void main() {
    // fragColor = vec4(our_color, 1.0);
    // fragColor = texture(our_tex, our_tex_coord);
    // fragColor = texture(our_tex, our_tex_coord) * vec4(our_color, 1.0);
    fragColor = mix(texture(our_tex, our_tex_coord), texture(our_tex1, our_tex_coord), 0.2);
}