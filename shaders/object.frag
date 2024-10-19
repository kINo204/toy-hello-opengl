#version 330 core
in vec3 norm;
in vec3 frag_pos;

uniform vec3 obj_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    vec3 ambient = light_color * 0.07; // We now use the same light_color for ambient.
    vec3 diffuse = light_color * 1.0 *
                    max(dot(normalize(norm), normalize(light_pos - frag_pos)), 0);
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 refl_dir = normalize(reflect(frag_pos - light_pos, norm));
    vec3 specular = light_color * 0.5 * pow(max(dot(view_dir, refl_dir), 0), 32);
    vec3 light = ambient + diffuse + specular;
    frag_color = vec4(obj_color * light, 1.0);
}