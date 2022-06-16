# version 460

#extension GL_ARB_shading_language_include : require

#include "/common/functionality/ibl_functions.glsl"

in vec2 tex_coords;

out vec4 fragColor;

void main()
{
    float NdotV = tex_coords.x;
    float roughness = tex_coords.y;

    vec3 V = vec3(sqrt(1.0 - NdotV * NdotV), 0.0, NdotV);
    vec3 N = vec3(0.0, 0.0, 1.0);
    float fa = 0.0;
    float fb = 0.0;
    for (uint i = 0; i < NUM_SAMPLES; i++) 
    {
        vec2 hammersley2Dresult = hammersley2D(i, NUM_SAMPLES);
        vec3 H = hemisphereSampleGGX(hammersley2Dresult.x, hammersley2Dresult.y, roughness);
        vec3 L = reflect(-V, H); 
        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        float VdotH = max(dot(V, H), 0.0);
        if (NdotL > 0.0) 
        {
            float V_pdf = SmithVSF(NdotL, NdotV, roughness) * VdotH * NdotL / NdotH;
            float Fc = pow(1.0 - VdotH, 5.0); // note: VdotH = LdotH
            fa += (1.0 - Fc) * V_pdf;
            fb += Fc * V_pdf;
        }
    }
    fragColor = vec4(4.0*fa/float(NUM_SAMPLES), 4.0*fb/float(NUM_SAMPLES), 1.0, 1.0);
}
