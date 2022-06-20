#version 460 core

#extension GL_ARB_shading_language_include : require

#include "/common/structs/lights.glsl"
#include "/common/uniforms/camera_uniform.glsl"
#include "/common/uniforms/lights_uniform.glsl"
#include "/common/functionality/lighting_functions.glsl"

out vec4 fragment_color;
in vec2 texture_coords;

uniform int mode;
uniform float shadow_bias;
uniform mat4 light_projection;
uniform mat4 light_view;

layout (binding = 0) uniform sampler2D g_diffuse;
layout (binding = 1) uniform sampler2D g_specular_smoothness;
layout (binding = 2) uniform sampler2D g_normal;
layout (binding = 3) uniform sampler2D g_position;
layout (binding = 4) uniform sampler2D g_emissive;
layout (binding = 5) uniform sampler2D shadow_map;

float CalculateShadow(vec4 fragment_position_from_light, sampler2D shadow_map_texture, float bias)
{
    // Perform perspective division:
    // NOTE: On the tutorials it is said that since we are using an orthographic projection,
    // this division is actually meaningless as w is always 1.0. However this step is necessary
    // when using perspective projection so we do it anyways to support perspective projection
    // out of the box.
    vec3 projection_coordinates = fragment_position_from_light.xyz / fragment_position_from_light.w;

    // The projection_coordinates will be returned in the range [-1, 1]. Because the depth from 
    // the shadow map is in the range [0,1] and we also want to use projection_coordinates from 
    // the shadow map, we transform the NDC coordinates to the range [0,1]:
    projection_coordinates = projection_coordinates * 0.5 + 0.5;

    // With these projected coordinats we can sample the depth map as the resulting [0,1] 
    // coordinates from projection_coordinates directly correspond to the transformed NDC coords
    // from the first shadow depth render pass. This calculation gives us the closest depth from
    // the light's perspective:
    float closest_depth = texture(shadow_map_texture, projection_coordinates.xy).r;

    // We can get the current depth at this fragment by retrieving the projected vector's z coords
    // which equals to the depth of this fragment from the light's perspective:
    float current_depth = projection_coordinates.z;

    // Now with closest_depth and current_depth in our hands, we check whether current_depth is 
    // higher than closest_depth, and if so this fragment is in shadow:
    float shadow = current_depth - bias > closest_depth ? 1.0f : 0.0f;

    return shadow; 
}

void main()
{

    vec3  fragment_diffuse = (texture(g_diffuse, texture_coords)).rgb;
    vec4  fragment_specular_smoothness = texture(g_specular_smoothness, texture_coords);
    vec3  fragment_normal = (texture(g_normal, texture_coords)).rgb;
    vec3  fragment_position = (texture(g_position, texture_coords)).rgb;
    vec4  fragment_position_from_light = light_projection * light_view * vec4(fragment_position, 1.0);
    vec3  fragment_emissive = (texture(g_emissive, texture_coords)).rgb;
    vec3  fragment_specular = fragment_specular_smoothness.rgb;
    float fragment_smoothness = fragment_specular_smoothness.a;
    vec3  view_direction = normalize(camera.pos - fragment_position);
    float shadow_map_depth = texture(shadow_map, texture_coords).r;
    
    if (mode == 0)
    {
        vec3 hdr_color = vec3(0.0);
        float fragment_shadow = CalculateShadow(fragment_position_from_light, shadow_map, shadow_bias);
        
        hdr_color += (1.0 -fragment_shadow) * DirectionalPBR(fragment_normal, view_direction, lights.directional, fragment_diffuse, fragment_specular, fragment_smoothness);
        
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

        
        hdr_color += GetAmbientLight(fragment_normal, reflect(-view_direction, fragment_normal), dot(fragment_normal, view_direction), pow(1.0 - fragment_smoothness, 2), fragment_diffuse, fragment_specular);

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