#version 330 core
in vec3 norm;
in vec3 frag_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 pos;
};
uniform Light light;

uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 refl_dir = normalize(reflect(frag_pos - light.pos, norm));
    vec3 color = vec3(0)
        + light.ambient * material.ambient
        + light.diffuse * material.diffuse * max(dot(normalize(norm), normalize(light.pos - frag_pos)), 0)
        + light.specular * pow(max(dot(view_dir, refl_dir), 0), material.shininess) * material.specular 
        ;
    frag_color =
        vec4(color, 1.0); // Real output
        /* Debugging outputs: */
        // vec4(material.shininess, 1.0f, 1.0f, 1.0f);
}