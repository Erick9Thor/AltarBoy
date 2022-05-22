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

// TODO: Be sure that this is fully correct:
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

// TODO: Check this function if it's fully correct:
// TODO: Add this function to the common file that will be created:
void CalculateSpecularDiffuseSmoothness(
    uint material_diffuse_flag,
    uint material_metallic_flag,
    uint material_specular_flag,
    uint material_is_metallic,
    uint material_smoothness_alpha,
    vec4 material_diffuse_color,
    vec4 material_specular_color,
    float material_metalness_value,
    float material_smoothness,
    vec4 texture_metal_color,
    vec4 texture_specular_color,
    vec4 texture_diffuse_color,
    inout float smoothness, 
    inout vec3 diffuse, 
    inout vec3 specular)
{
    vec4 diffuse_temp = material_diffuse_flag * pow(texture_diffuse_color, vec4(2.2)) + // true
                        (1 - material_diffuse_flag) * material_diffuse_color;           // false

    if (material_is_metallic > 0)
    {
        // If it doesn't have metalness texture it adds the metalness value of the material:
        float metalness_mask = material_metallic_flag * texture_metal_color.r +         // true
                               (1 - material_metallic_flag) * material_metalness_value; // false
        
        // Cd:
        diffuse = diffuse_temp.rgb * (1 - metalness_mask);
        // f0:
        specular = vec3(0.04) * (1 - metalness_mask) + diffuse_temp.rgb * metalness_mask;

        smoothness = material_smoothness * ((material_smoothness_alpha * texture_metal_color.a) + // true
                     ((1 - material_smoothness_alpha) * diffuse_temp.a ));                        // false
    }
    else 
    {
        // Cd:
        diffuse = diffuse_temp.rgb;

        // If the flag is true (1) it will paint texture specular color, otherwise material specular color:
        vec4 color_specular = (material_specular_flag * texture_specular_color) +       // true
                              ((1 - material_specular_flag) * material_specular_color); // false
        
        // f0:
        specular = color_specular.rgb;

        smoothness = material_smoothness * ((material_smoothness_alpha * color_specular.a) + // true
                     ((1 - material_smoothness_alpha)* diffuse_temp.a ));                    // false
    } 
}
 
void main()
{
    // Temporary values that will be calculated:
    float smoothness;
    vec3 diffuse;
    vec3 specular;
    
    // Calculate specular color, diffuse color and smoothness based on material properties and textures:
    CalculateSpecularDiffuseSmoothness(
        material.diffuse_flag,
        material.metallic_flag,
        material.specular_flag,
        material.is_metallic,
        material.smoothness_alpha,
        material.diffuse_color,
        material.specular_color,
        material.metalness_value,
        material.smoothness,
        texture(textures[METALLIC_SAMPLER], fragment.tex_coord),
        texture(textures[SPECULAR_SAMPLER], fragment.tex_coord),
        texture(textures[DIFFUSE_SAMPLER], fragment.tex_coord),
        smoothness, 
        diffuse, 
        specular);

    // Store diffuse color in rgb channels of g buffer texture for diffuse: 
    g_diffuse.rgb = diffuse;
    g_diffuse.a = 1.0f;

    // Store specular color in rgb channels of g buffer texture for specular and smoothness:
    g_specular_smoothness.rgb = specular;
    // Store smoothness in a channel of g buffer texture for specular and smoothness:
    g_specular_smoothness.a = smoothness;

    // Store the fragment normal in rgb channels of g buffer texture for normal:
    g_normal.xyz = CalculateNormal(
        material.normal_flag, 
        texture(textures[NORMAL_SAMPLER], fragment.tex_coord).rgb, 
        fragment.normal, 
        fragment.tangent); 

    // Store the fragment position in rgb channels of g buffer texture for position:
    g_position.xyz = fragment.pos.xyz;
}