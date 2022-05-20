#version 460 core

out vec4 fragment_color;

in vec2 texture_coords;

uniform float mode;
uniform sampler2D g_diffuse;
uniform sampler2D g_specular_smoothness;
uniform sampler2D g_normal;
uniform sampler2D g_position;

// TODO: Add lights and stuff.

void main()
{
    vec3  fragment_diffuse = texture(g_diffuse, texture_coords).rgb;
    vec3  fragment_specular = texture(g_specular_smoothness, texture_coords).rgb;
    float fragment_smoothness = texture(g_specular_smoothness, texture_coords).a;
    vec3  fragment_normal = normalize(texture(g_normal, texture_coords).rgb);
    vec3  fragment_position = texture(g_position, texture_coords).rgb;
    
    if (mode == 0.0f)
    {
        fragment_color.rgb = fragment_diffuse;
    }
    else if (mode == 1.0f)
    {
        fragment_color.rgb = fragment_specular;
    }
    else if (mode == 2.0f)
    {
        fragment_color.rgb = vec3(fragment_smoothness, fragment_smoothness, fragment_smoothness);
    }
    else if (mode == 3.0f)
    {
        fragment_color.rgb = fragment_normal;
    }
    else
    {
        fragment_color.rgb = fragment_position;
    }
}