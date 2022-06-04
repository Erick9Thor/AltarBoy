#version 460 core

#extension GL_ARB_shading_language_include : require

#include "/common/vertex_data.glsl"
#include "/common/material.glsl"
#include "/common/material_property_functions.glsl"
#include "/common/uniforms/material_and_texture_uniforms.glsl"

// G Buffer textures:
layout (location = 0) out vec4 g_diffuse;
layout (location = 1) out vec4 g_specular_smoothness;
layout (location = 2) out vec4 g_normal;
layout (location = 3) out vec4 g_position;
layout (location = 4) out vec4 g_emissive;

// Inputs
in VertexData fragment;
in flat uint instance;

void main()
{
    Material material = materialsBuffer.materials[instance];

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
        texture(allMyTextures[material.metallic_map.texIndex], vec3(fragment.tex_coord, material.metallic_map.layerIndex)),
        texture(allMyTextures[material.specular_map.texIndex], vec3(fragment.tex_coord, material.specular_map.layerIndex)),
        texture(allMyTextures[material.diffuse_map.texIndex], vec3(fragment.tex_coord, material.diffuse_map.layerIndex)),
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
        texture(allMyTextures[material.normal_map.texIndex], vec3(fragment.tex_coord, material.normal_map.layerIndex)).rgb, 
        fragment.normal, 
        fragment.tangent); 

    // Store the fragment position in rgb channels of g buffer texture for position:
    g_position.xyz = fragment.pos.xyz;

    // Store the fragment emissive color in rgb channels of g buffer texture for emissive:
    g_emissive.xyz = CalculateEmissive(
        texture(allMyTextures[material.emissive_map.texIndex], vec3(fragment.tex_coord, material.emissive_map.layerIndex)).rgb,
        material.emissive_color,
        material.emissive_flag
    );
}