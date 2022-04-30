# version 440

#define MAX_BONES 64

layout(location=0) in vec3 position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_tex_coord;
layout(location=3) in vec3 in_tangent;
layout(location=4) in ivec4 bone_indices;
layout(location=5) in vec4 bone_weights;

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
    vec3 pos;
} camera;

// Outputs
struct VertexData
{
    vec3 normal;
    vec3 tangent;
    vec3 pos;
    vec2 tex_coord;
};
out VertexData fragment;

uniform mat4 model;

vec3 aux_pos;
vec3 aux_normal;

// SKINING

layout(std140, row_major) uniform Skining
{
    mat4 palette[MAX_BONES];
};

void TransformOutput()
{
    mat4 skin_transform = palette[bone_indices[0]]*bone_weights[0]+palette[bone_indices[1]]*bone_weights[1]+
                          palette[bone_indices[2]]*bone_weights[2]+palette[bone_indices[3]]*bone_weights[3];

    aux_pos = (skin_transform*vec4(position, 1.0)).xyz;
    aux_normal = (skin_transform*vec4(in_normal, 0.0)).xyz;
}

void main()
{

    TransformOutput();

    gl_Position = camera.proj * camera.view *  model * vec4(aux_pos, 1.0);

    fragment.normal = transpose(inverse(mat3(model))) * aux_normal;
    fragment.tangent = transpose(inverse(mat3(model))) * in_tangent;
    fragment.pos = vec3(model * vec4(aux_pos, 1.0));
    fragment.tex_coord = in_tex_coord;   
}