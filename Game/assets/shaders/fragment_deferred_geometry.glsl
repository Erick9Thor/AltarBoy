#version 460 core

#define DIFFUSE_SAMPLER 0
#define SPECULAR_SAMPLER 1
#define NORMAL_SAMPLER 2
#define METALLIC_SAMPLER 3
#define EMISSIVE_SAMPLER 4
#define N_2D_SAMPLERS 5

layout (location = 0) out vec4 g_diffuse;
layout (location = 1) out vec4 g_specular_smoothness;
layout (location = 2) out vec4 g_normal;
layout (location = 3) out vec4 g_position;

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

mat3 CreateTangentSpace(const vec3 normal, const vec3 tangent)
{
	vec3 ortho_tangent = normalize(tangent - dot(tangent, normal) * normal); // Gram-Schmidt
	vec3 bitangent = cross(normal, ortho_tangent);
	return mat3(tangent, bitangent, normal);
}

// TODO: Be sure that this is correct.
vec3 CalculateNormal(uint normal_flag, vec3 texture_normal_coords, vec3 fragment_normal_coords, vec3 fragment_tangent)
{
    vec3 normal = normalize(fragment_normal_coords);

    if (normal_flag > 0)
    {
        mat3 tbn = CreateTangentSpace(normal, normalize(fragment_tangent));
        vec3 texture_normal = normalize(texture_normal_coords.xyz);
	    vec3 fragment_normal = tbn * (texture_normal * 2.0 - 1.0);
	    normal = normalize(fragment_normal);
    }

    return normal;
}
 
void main()
{
    // TODO: Add all material specific things to g buffer.

    // Store diffuse color in rgb channels of g buffer texture for diffuse: 
    g_diffuse.rgb = material.diffuse_flag > 0 
         ? (pow(texture(textures[DIFFUSE_SAMPLER], fragment.tex_coord), vec4(2.2))).rgb 
         : material.diffuse_color.rgb;
    g_diffuse.a = 1.0f;

    // Store specular color in rgb channels of g buffer texture for specular and smoothness:
    g_specular_smoothness.rgb = material.specular_flag > 0 
        ? texture(textures[SPECULAR_SAMPLER], fragment.tex_coord).rgb
        : material.specular_color.rgb;
    // Store smoothness in a channel of g buffer texture for specular and smoothness:
    g_specular_smoothness.a = material.smoothness;

    // Store the fragment normal in rgb channels of g buffer texture for normal:
    g_normal.xyz = CalculateNormal(
        material.normal_flag, 
        texture(textures[NORMAL_SAMPLER], fragment.tex_coord).rgb, 
        fragment.normal, 
        fragment.tangent); 

    // Store the fragment position in rgb channels of g buffer texture for position:
    g_position.xyz = fragment.pos.xyz;
}