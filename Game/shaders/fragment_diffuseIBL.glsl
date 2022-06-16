# version 460

#extension GL_ARB_shading_language_include : require

#include "/common/functionality/ibl_functions.glsl"

in vec3 tex_coords;

layout (binding = 0) uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
    vec3 irradiance = vec3(0.0);
    vec3 normal = normalize(tex_coords);
    mat3 tangentSpace = computeTangetSpace(normal);
    for(int i=0; i< NUM_SAMPLES; ++i)
    {
        vec2 rand_value = hammersley2D(i, NUM_SAMPLES);
        vec3 sample_dir = tangentSpace * hemisphereSample(rand_value[0], rand_value[1]); 
        irradiance += texture(skybox, sample_dir).rgb;
    }
    fragColor = vec4(irradiance*(1.0/float(NUM_SAMPLES)), 1.0);
}