#version 460

#extension GL_ARB_shading_language_include : require

#include "/common/uniforms/camera_uniform.glsl"

// Inputs
in vec2 tex_coord;

uniform sampler2D diffuse;
uniform int diffuse_flag;
uniform vec4 img_color;

// Outputs
out vec4 color;

void main()
{
    // vec4(pow(color.rgb, vec3(GAMMA)), color.a);
    color = img_color;
    if (diffuse_flag > 0)
        color = vec4(texture2D(diffuse, tex_coord));
}