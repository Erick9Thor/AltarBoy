#version 460 

out float out_fragment_color;
in vec2 texture_coords;

// layout (binding = 0) uniform sampler2D g_diffuse;
layout (binding = 0) uniform sampler2D texture_noise;
layout (binding = 1) uniform sampler2D g_normal;
layout (binding = 2) uniform sampler2D g_position;

uniform float frame_buffer_width;
uniform float frame_buffer_height;
uniform int kernel_size;
uniform float radius;
uniform float bias;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 samples[64];

const float noise_texture_size = 4.0;
const float noise_texture_size_inv = 1.0 / noise_texture_size;
const vec2 noise_scale = vec2(
    frame_buffer_width * noise_texture_size_inv, 
    frame_buffer_height * noise_texture_size_inv);

void main()
{
    vec3 fragment_position = texture(g_position, texture_coords).xyz;
    vec3 fragment_normal = normalize(texture(g_normal, texture_coords).xyz);
    vec3 random_vec = normalize(texture(texture_noise, texture_coords * noise_scale).xyz);

    // Create TBN change-of-basis matrix. From tangent-space to view-space:
    vec3 tangent = normalize(random_vec - fragment_normal * dot(random_vec, fragment_normal));
    vec3 bitangent = normalize(cross(fragment_normal, tangent));
    mat3 tbn = mat3(tangent, bitangent, fragment_normal);

    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < 64; ++i)
    {
        // Get sample position:
        vec3 sample_position = tbn * samples[i]; // from tangent to view-space
        sample_position = fragment_position + sample_position * radius; 
        
        // Project sample position (to sample texture) (to get position on screen/texture):
        vec4 offset = vec4(sample_position, 1.0);
        offset = projection * view * offset; // From view to clip-space
        offset.xyz /= offset.w; // Perspective division
        offset.xyz = offset.xyz * 0.5 + 0.5; // Transform to range 0.0 - 1.0
        
        // Get sample depth:
        float sample_depth = texture(g_position, offset.xy).z; // Get depth value of kernel sample

        // range check & accumulate
        // float range_check = abs(fragment_position.z - sample_depth) < radius ? 1.0 : 0.0;
        // occlusion += (sample_depth >= sample_position.z + bias ? 1.0 : 0.0) * range_check;           
        occlusion += (sample_depth >= sample_position.z + bias ? 1.0 : 0.0);
    }

    // occlusion = pow(occlusion, 2.0);
    out_fragment_color = 1.0 - (occlusion / 64);
}