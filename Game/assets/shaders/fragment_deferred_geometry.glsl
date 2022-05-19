#version 460 core

#define DIFFUSE_SAMPLER 0
#define SPECULAR_SAMPLER 1
#define NORMAL_SAMPLER 2
#define METALLIC_SAMPLER 3
#define EMISSIVE_SAMPLER 4
#define N_2D_SAMPLERS 5

// layout (location = 0) out vec4 g_diffuse;
// layout (location = 1) out vec4 g_specular_smoothness;
layout (location = 0) out vec3 g_normal;
layout (location = 1) out vec3 g_position;

layout(std140, binding = 1) uniform Material
{
    vec4 diffuse_color;
    vec4 specular_color;
    vec4 emissive_color;
    uint diffuse_flag;
    uint specular_flag;
    uint normal_flag;
    uint metallic_flag;
    uint emissive_flag;
    float smoothness;
    float metalness_value;
    uint is_metallic;
    uint smoothness_alpha;
    uint is_transparent;
} material;

// Inputs
struct VertexData
{
    vec3 normal;
    vec3 tangent;
    vec3 pos;
    vec2 tex_coord;
};

in VertexData fragment;

uniform sampler2D textures[N_2D_SAMPLERS];
 
void main()
{
    // TODO: Add all material specific things to g buffer.

    // // Store diffuse color in rgb channels of g buffer texture for diffuse: 
    // g_diffuse.rgb = material.diffuse_flag > 0 
    //     ? texture(textures[DIFFUSE_SAMPLER], fragment.tex_coord).rgb 
    //     : material.diffuse_color.rgb;
    // g_diffuse.a = 1.0f;

    // // Store specular color in rgb channels of g buffer texture for specular and smoothness:
    // g_specular_smoothness.rgb = material.specular_flag > 0 
    //     ? texture(textures[SPECULAR_SAMPLER], fragment.tex_coord).rgb
    //     : material.specular_color.rgb;
    // // Store smoothness in a channel of g buffer texture for specular and smoothness:
    // g_specular_smoothness.a = material.smoothness;

    // Store the fragment normal in rgb channels of g buffer texture for normal:
    g_normal = normalize(fragment.normal);

    // Store the fragment position in rgb channels of g buffer texture for position:
    g_position = fragment.pos;
}