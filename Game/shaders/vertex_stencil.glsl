#version 460

#extension GL_ARB_shading_language_include : require

#include "/common/uniforms/camera_uniform.glsl"

layout(location=0) in vec3 position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_tex_coord;

uniform mat4 model;

void main()
{
    mat4 outline_scale = mat4(1.05, 0.0, 0.0, 0.0,
                              0.0, 1.05, 0.0, 0.0,
                              0.0, 0.0, 1.05, 0.0,
                              0.0, 0.0, 0.0, 1.0);
    mat4 scaled = model * outline_scale;
    gl_Position = camera.proj * camera.view * scaled * vec4(position, 1.0);
}