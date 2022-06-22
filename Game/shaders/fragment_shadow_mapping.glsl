#version 460

out vec4 variance_shadow_map;

void main()
{
    float depth = gl_FragCoord.z;

    float moment1 = depth;
    float moment2 = moment1 * moment1;

    // Adjusting moments (this is sort of bias per pixel) using partial derivative
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moment2 += 0.25*(dx*dx+dy*dy);

    variance_shadow_map = vec4(moment1, moment2, 0.0, 0.0);

    // variance_shadow_map = vec4(gl_FragCoord.z, gl_FragCoord.z * gl_FragCoord.z, 0.0, 0.0); 
}