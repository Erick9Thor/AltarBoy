#version 460 core

out vec4 fragment_color;

in vec2 texture_coords;

uniform int mode;
layout (binding = 0) uniform sampler2D g_specular_smoothness;
layout (binding = 1) uniform sampler2D g_diffuse;
layout (binding = 2) uniform sampler2D g_normal;
layout (binding = 3) uniform sampler2D g_position;

// TODO: Add lights and stuff.

void main()
{
    vec3  fragment_diffuse = (texture(g_diffuse, texture_coords)).rgb;
    vec3  fragment_specular = (texture(g_specular_smoothness, texture_coords)).rgb;
    float fragment_smoothness = (texture(g_specular_smoothness, texture_coords)).a;
    vec3  fragment_normal = (texture(g_normal, texture_coords)).rgb;
    vec3  fragment_position = (texture(g_position, texture_coords)).rgb;

    if (mode == 0)
    {
        fragment_color = vec4(fragment_diffuse, 1.0f);
    }
    else if (mode == 1)
    {
        fragment_color = vec4(fragment_specular, 1.0f);
    }
    else if (mode == 2)
    {
        fragment_color = vec4(fragment_smoothness, fragment_smoothness, fragment_smoothness, 1.0f);
    }
    else if (mode == 3)
    {
        fragment_color = vec4(fragment_normal, 1.0f);
    }
    else
    {
        fragment_color = vec4(fragment_position, 1.0f);
    }
}