# version 460

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_tex_coord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv0;

void main()
{
	gl_Position = vec4(in_position.xyz , 1.0);
	uv0 = in_tex_coord;
}