#version 330 core
in vec3 norm;
in vec3 apos;
in vec3 frag_pos;
in vec4 frag_pos_light_space[2];

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

uniform sampler2D shadow_map;
uniform sampler2D shadow_map2;
uniform samplerCube envmap;
uniform sampler2D tex;

uniform vec3 camera_pos;

out vec4 frag_color;

void main() {
    vec3 view_dir = normalize(camera_pos - frag_pos);

    float shadow = 0, shadow2 = 0;

    vec3 proj_coords = frag_pos_light_space[0].xyz / frag_pos_light_space[0].w;
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

    vec3 proj_coords2 = frag_pos_light_space[1].xyz / frag_pos_light_space[1].w;
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

    vec3 refl_dir = normalize(reflect(frag_pos - light[0].pos, norm));
    vec3 refl_dir2 = normalize(reflect(frag_pos - light[1].pos, norm));

    vec3 color = vec3(0)
        + light[0].ambient * material.ambient
        + light[1].ambient * material.ambient
        + (1 - shadow) * (
        light[0].diffuse * material.diffuse * max(dot(normalize(norm), normalize(light[0].pos - frag_pos)), 0)
        + light[0].specular * pow(max(dot(view_dir, refl_dir), 0), material.shininess) * material.specular 
        )
        + (1 - shadow2) * (
        light[1].diffuse * material.diffuse * max(dot(normalize(norm), normalize(light[1].pos - frag_pos)), 0)
        + light[1].specular * pow(max(dot(view_dir, refl_dir2), 0), material.shininess) * material.specular
        )
        ;
    if (dot(norm, vec3(-1, 0, 0)) > 0.f) {
        // Put envmap here.
        frag_color = vec4(vec3(texture(envmap, vec3(-apos.x,-apos.y,apos.z))), 1);
        return;
    } else if (dot(norm, vec3(1, 0, 0)) > 0.f) {
        frag_color = texture(tex, vec2(0.5f-apos.z, -apos.y));
        return;
    } else {
        frag_color =
            vec4(color, material.alpha); // Real output
            /* Debugging outputs: */
            // vec4(vec3(texture(shadow_map2, proj_coords2.xy).r), 1.0);
            // vec4(vec3(texture(envmap, -apos)), 1);
    }
}