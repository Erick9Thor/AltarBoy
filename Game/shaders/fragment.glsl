# version 460

#extension GL_ARB_shading_language_include : require

#include "/common/vertex_data.glsl"
#include "/common/material.glsl"
#include "/common/material_property_functions.glsl"
#include "/common/lights.glsl"
#include "/common/lighting_functions.glsl"
#include "/common/uniforms/camera_uniform.glsl"

layout(std140, binding = 2) uniform Lights
{
    AmbientLight ambient;
    DirLight directional;
    PointLight points[MAX_POINT_LIGHTS];
    SpotLight spots[MAX_SPOT_LIGHTS];
    uint n_points;
    uint n_spots;
} lights;

// Texture Batching
uniform sampler2DArray allMyTextures[gl_MaxTextureImageUnits-8];

readonly layout(std430, binding = 1) buffer Materials {
    Material materials[];
} materialsBuffer;

// Inputs
in VertexData fragment;
in flat uint instance;

// Outputs
out vec4 color;

void main()
{
    Material material = materialsBuffer.materials[instance];
    
    float smoothness;
    vec3 diffuse;
    vec3 specular;

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


    vec3 normal = CalculateNormal(
        material.normal_flag, 
        texture(allMyTextures[material.normal_map.texIndex], vec3(fragment.tex_coord, material.normal_map.layerIndex)).rgb, 
        fragment.normal, 
        fragment.tangent); 
    
    vec3 view_direction = normalize(camera.pos - fragment.pos);
    
    vec3 hdr_color = vec3(0.0);
    hdr_color += DirectionalPBR(normal, view_direction, lights.directional, diffuse, specular, smoothness);
    
    for(uint i=0; i<lights.n_points; ++i)
    {
        hdr_color +=  PositionalPBR(fragment.pos, normal, view_direction, lights.points[i], diffuse, specular, smoothness);
    }

    for(uint i=0; i<lights.n_spots; ++i)
    {
        hdr_color +=  SpotPBR(fragment.pos, normal, view_direction, lights.spots[i], diffuse, specular, smoothness);
    }   

    hdr_color += diffuse * lights.ambient.color.rgb * lights.ambient.intensity;

    // Calculate emissive color:
    vec3 emissive = CalculateEmissive(
        texture(allMyTextures[material.emissive_map.texIndex], vec3(fragment.tex_coord, material.emissive_map.layerIndex)).rgb,
        material.emissive_color,
        material.emissive_flag
    );

    hdr_color += emissive;

    // Reinhard tone mapping
    vec3 ldr_color = hdr_color / (hdr_color + vec3(1.0));

    // Gamma correction & alpha from diffuse texture if it is transparent
    color = vec4(pow(ldr_color.rgb, vec3(1.0/2.2)), material.diffuse_color.a * material.is_transparent + (1 - material.is_transparent));
}