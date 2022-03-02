# version 460

layout(location=0) in vec3 position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_tex_coord;
layout(location=3) in uint instance_idx;

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
    vec3 pos;
} camera;

readonly layout(std430, row_major, binding = 3) buffer Transforms
{
 mat4 models[];
};

uniform mat4 model;

// Outputs
struct VertexData
{
    vec3 normal;
    vec3 pos;
    vec2 tex_coord;
};
out VertexData fragment;
out flat uint instance;

void main()
{
    uint a = instance_idx;
    gl_Position = camera.proj * camera.view *  models[a] * vec4(position, 1.0);

    fragment.normal = transpose(inverse(mat3(models[a]))) * in_normal;
    fragment.pos = vec3(models[a] * vec4(position, 1.0));
    fragment.tex_coord = in_tex_coord;
    instance = instance_idx;
}