#version 330 core
in vec3 norm;
in vec3 frag_pos;
in vec4 frag_pos_light_space;
in vec4 frag_pos_light_space2;

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
uniform Light light2;

uniform sampler2D shadow_map;
uniform sampler2D shadow_map2;

uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    float shadow = 0, shadow2 = 0;

    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float current_depth = proj_coords.z;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += current_depth - 0.005f > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    vec3 proj_coords2 = frag_pos_light_space2.xyz / frag_pos_light_space2.w;
    proj_coords2 = proj_coords2 * 0.5 + 0.5;
    float current_depth2 = proj_coords2.z;
    vec2 texelSize2 = 1.0 / textureSize(shadow_map2, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(shadow_map2, proj_coords2.xy + vec2(x, y) * texelSize2).r; 
            shadow2 += current_depth2 - 0.005f > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow2 /= 9.0;

    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 refl_dir = normalize(reflect(frag_pos - light.pos, norm));
    vec3 refl_dir2 = normalize(reflect(frag_pos - light2.pos, norm));

    vec3 color = vec3(0)
        + light.ambient * material.ambient
        + light2.ambient * material.ambient
        + (1 - shadow) * (
        light.diffuse * material.diffuse * max(dot(normalize(norm), normalize(light.pos - frag_pos)), 0)
        + light.specular * pow(max(dot(view_dir, refl_dir), 0), material.shininess) * material.specular 
        )
        + (1 - shadow2) * (
        light2.diffuse * material.diffuse * max(dot(normalize(norm), normalize(light2.pos - frag_pos)), 0)
        + light2.specular * pow(max(dot(view_dir, refl_dir2), 0), material.shininess) * material.specular
        )
        ;
    frag_color =
        vec4(color, 1.0); // Real output
        /* Debugging outputs: */
        // vec4(vec3(texture(shadow_map2, proj_coords2.xy).r), 1.0);
}