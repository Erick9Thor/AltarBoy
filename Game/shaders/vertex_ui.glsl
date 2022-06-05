#version 460

#extension GL_ARB_shading_language_include : require

#include "/common/uniforms/camera_uniform.glsl"

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_tex_coord;

uniform mat4 model;

// Outputs
out vec2 tex_coord;

void main()
{
    gl_Position = camera.proj * camera.view * model * vec4(in_position, 1.0);
    tex_coord = in_tex_coord;
}