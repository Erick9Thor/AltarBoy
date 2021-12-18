# version 330
layout(location=0) in vec3 position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normal;
out vec3 frag_world_position;
out vec2 tex_coord;

void main()
{
    gl_Position = proj * view *  model * vec4(position, 1.0);

    tex_coord = in_tex_coord;
    frag_world_position = vec3(model * vec4(position, 1.0));
    normal = transpose(inverse(mat3(model))) * in_normal;
}