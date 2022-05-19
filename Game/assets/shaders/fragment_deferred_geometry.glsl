#version 460 core

#define DIFFUSE_SAMPLER 0
#define SPECULAR_SAMPLER 1
#define NORMAL_SAMPLER 2
#define METALLIC_SAMPLER 3
#define EMISSIVE_SAMPLER 4
#define N_2D_SAMPLERS 5

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_specular;

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

    // Store the fragment position in our first g buffer texture:
    g_position = fragment.pos;
    // Store the fragment normal in the second g buffer texture:
    g_normal = normalize(fragment.normal);
    // Store the fragment color in r,g and b of our third g buffer texture:
    g_albedo_specular.rgb = texture(textures[DIFFUSE_SAMPLER], fragment.tex_coord).rgb;
    // Store the specular intensity in a of our third g buffer texture:
    g_albedo_specular.a = texture(textures[SPECULAR_SAMPLER], fragment.tex_coord).r;
}