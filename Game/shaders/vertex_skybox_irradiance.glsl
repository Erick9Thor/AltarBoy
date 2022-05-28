# version 460
layout (location = 0) in vec3 pos;

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
};

out vec3 tex_coords;

void main ()
{
    tex_coords = vec3(-pos.x, pos.yz);
    gl_Position = (proj * vec4(mat3(view) * pos, 1.0)).xyww;
}