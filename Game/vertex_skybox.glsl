# version 330
layout (location=0) in vec3 position;

out vec3 tex_coords;

uniform mat4 view;
uniform mat4 proj;

void main ()
{
    gl_Position = (proj * vec4(mat3(view) * position, 1.0)).xyww;
    tex_coords = position;    
}