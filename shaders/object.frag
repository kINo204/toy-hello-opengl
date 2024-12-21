#version 330 core
in vec3 norm;
in vec3 frag_pos;
in vec4 frag_pos_light_space;

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

uniform sampler2D shadow_map;

uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 refl_dir = normalize(reflect(frag_pos - light.pos, norm));

    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float current_depth = proj_coords.z;
    float shadow;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += current_depth - 0.001f > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    vec3 color = vec3(0)
        + light.ambient * material.ambient
        + (1.0f - shadow) * light.diffuse * material.diffuse * max(dot(normalize(norm), normalize(light.pos - frag_pos)), 0)
        + (1.0f - shadow) * light.specular * pow(max(dot(view_dir, refl_dir), 0), material.shininess) * material.specular 
        ;
    frag_color =
        vec4(color, 1.0); // Real output
        /* Debugging outputs: */
        // vec4(vec3(current_depth), 1.0);
}