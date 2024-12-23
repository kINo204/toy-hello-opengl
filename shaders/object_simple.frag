#version 330 core
in vec3 norm;
in vec3 frag_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 pos;
};
uniform Light light[2];

uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 refl_dir = normalize(reflect(frag_pos - light[0].pos, norm));
    vec3 refl_dir2 = normalize(reflect(frag_pos - light[1].pos, norm));

    vec3 color = vec3(0)
        + light[0].ambient * material.ambient + light[1].ambient * material.ambient
        + light[0].diffuse * material.diffuse * max(dot(normalize(norm), normalize(light[0].pos - frag_pos)), 0)
        + light[0].specular * pow(max(dot(view_dir, refl_dir), 0), material.shininess) * material.specular 
        + light[1].diffuse * material.diffuse * max(dot(normalize(norm), normalize(light[1].pos - frag_pos)), 0)
        + light[1].specular * pow(max(dot(view_dir, refl_dir2), 0), material.shininess) * material.specular
        ;
    frag_color =
        vec4(color, material.alpha);
}