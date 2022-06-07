#version 460

#extension GL_ARB_shading_language_include : require

#include "/common/uniforms/camera_uniform.glsl"

// Outputs
out vec4 color;

void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
}