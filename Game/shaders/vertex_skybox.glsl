#version 460

#extension GL_ARB_shading_language_include : require

#define CAMERA_WITHOUT_POSITION
#include "/common/uniforms/camera_uniform.glsl"

layout (location=0) in vec3 position;

out vec3 tex_coords;

void main ()
{
    gl_Position = (camera.proj * vec4(mat3(camera.view) * position, 1.0)).xyww;
    tex_coords = position;
    tex_coords.y = -tex_coords.y;
}