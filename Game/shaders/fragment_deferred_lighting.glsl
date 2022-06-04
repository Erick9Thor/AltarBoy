#version 460 core

#extension GL_ARB_shading_language_include : require

#include "/common/structs/lights.glsl"
#include "/common/uniforms/camera_uniform.glsl"
#include "/common/uniforms/lights_uniform.glsl"
#include "/common/functionality/lighting_functions.glsl"

out vec4 fragment_color;
in vec2 texture_coords;

uniform int mode;

layout (binding = 0) uniform sampler2D g_diffuse;
layout (binding = 1) uniform sampler2D g_specular_smoothness;
layout (binding = 2) uniform sampler2D g_normal;
layout (binding = 3) uniform sampler2D g_position;
layout (binding = 4) uniform sampler2D g_emissive;

void main()
{
    vec3  fragment_diffuse = (texture(g_diffuse, texture_coords)).rgb;
    vec3  fragment_specular = (texture(g_specular_smoothness, texture_coords)).rgb;
    float fragment_smoothness = (texture(g_specular_smoothness, texture_coords)).a;
    vec3  fragment_normal = (texture(g_normal, texture_coords)).rgb;
    vec3  fragment_position = (texture(g_position, texture_coords)).rgb;
    vec3  fragment_emissive = (texture(g_emissive, texture_coords)).rgb;
    vec3  view_direction = normalize(camera.pos - fragment_position);
    
    if (mode == 0)
    {
        vec3 hdr_color = vec3(0.0);
        hdr_color += DirectionalPBR(fragment_normal, view_direction, lights.directional, fragment_diffuse, fragment_specular, fragment_smoothness);
        
        for(uint i=0; i<lights.n_points; ++i)
        {
            if (distance(lights.points[i].position.xyz, fragment_position) <= lights.points[i].radius)
            {
                hdr_color +=  PositionalPBR(fragment_position, fragment_normal, view_direction, lights.points[i], fragment_diffuse, fragment_specular, fragment_smoothness);
            }
        }

        for(uint i=0; i<lights.n_spots; ++i)
        {
            hdr_color +=  SpotPBR(fragment_position, fragment_normal, view_direction, lights.spots[i], fragment_diffuse, fragment_specular, fragment_smoothness);
        }

        hdr_color += fragment_diffuse * lights.ambient.color.rgb * lights.ambient.intensity;

        hdr_color += fragment_emissive;

        // Reinhard tone mapping
        vec3 ldr_color = hdr_color / (hdr_color + vec3(1.0));

        // Gamma correction & alpha from diffuse texture if it is transparent
        fragment_color = vec4(pow(ldr_color.rgb, vec3(1.0/2.2)), 1.0f);
    }

    // Debug modes:

    else if (mode == 1)
    {
        fragment_color = vec4(fragment_diffuse, 1.0f);
    }
    else if (mode == 2)
    {
        fragment_color = vec4(fragment_specular, 1.0f);
    }
    else if (mode == 3)
    {
        fragment_color = vec4(fragment_smoothness, 0.0f, fragment_smoothness, 1.0f);
    }
    else if (mode == 4)
    {
        fragment_color = vec4(fragment_normal, 1.0f);
    }
    else if (mode == 5)
    {
        fragment_color = vec4(fragment_position, 1.0f);
    }
    else
    {
        fragment_color = vec4(fragment_emissive, 1.0f);
    }
}