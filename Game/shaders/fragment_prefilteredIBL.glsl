# version 460

#extension GL_ARB_shading_language_include : require

#include "/common/functionality/ibl_functions.glsl"

in vec3 tex_coords;

layout (binding = 0) uniform samplerCube skybox;
uniform float roughness;

out vec4 fragColor;

void main()
{
    vec3 R = normalize(tex_coords);
    vec3 N = R, V = R;
    vec3 color = vec3(0.0);
    float weight = 0.0;
    mat3 tangentSpace = computeTangetSpace(N);
    for (int i = 0; i < NUM_SAMPLES; ++i) 
    {
        vec2 rand_value = hammersley2D(i, NUM_SAMPLES);
        vec3 H = tangentSpace * hemisphereSampleGGX(rand_value[0], rand_value[1], roughness);
        vec3 L = reflect(-V, H); 
        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0) 
        {
            color += texture(skybox, L).rgb * NdotL;
            weight += NdotL;
        }
    }
    fragColor = vec4(color / weight, 1.0);
}