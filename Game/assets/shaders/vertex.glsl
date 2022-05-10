# version 460

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_tex_coord;
layout(location=3) in vec3 in_tangent;
layout(location=4) in ivec4 in_bone_indices;
layout(location=5) in vec4 in_bone_weights;

#define MAX_BONES 64

uniform bool has_bones;

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
    vec3 pos;
} camera;

uniform mat4 model;

/*layout(std140, row_major) uniform Skining
{
    mat4 palette[MAX_BONES];
};*/

uniform mat4 palette[MAX_BONES];

// Outputs
struct VertexData
{
    vec3 normal;
    vec3 tangent;
    vec3 pos;
    vec2 tex_coord;
};
out VertexData fragment;

void main()
{
    vec4 position = vec4(in_position, 1.0);
    vec4 normal = vec4(in_normal, 0.0);
    vec4 tangent = vec4(in_tangent, 0.0);

    if (has_bones)
    { 
        mat4 skin_transform = palette[in_bone_indices[0]] * in_bone_weights[0] + palette[in_bone_indices[1]] * in_bone_weights[1] +
            palette[in_bone_indices[2]] * in_bone_weights[2] + palette[in_bone_indices[3]] * in_bone_weights[3];

        position = (skin_transform*vec4(in_position, 1.0));
        normal = (skin_transform*vec4(in_normal, 0.0));
        tangent = (skin_transform*vec4(in_tangent, 0.0));
    }

    gl_Position = camera.proj * camera.view *  model * position;

    fragment.pos = vec3(model * position);
    fragment.normal = normalize(transpose(inverse(mat3(model))) * normal.xyz);
    fragment.tangent = transpose(inverse(mat3(model))) * tangent.xyz;
    fragment.tex_coord = in_tex_coord;
}