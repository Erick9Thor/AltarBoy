# version 460

#extension GL_ARB_shading_language_include : require

#include "/common/uniforms/camera_uniform.glsl"
#include "/common/structs/vertex_data.glsl"

layout(location=0) in vec3 in_position;

uniform mat4 model;
uniform int ignore_camera;

out VertexData fragment;

void main()
{
	// This shader is unused atm
	gl_Position = (camera.proj * camera.view * model * vec4(in_position.xyz , 1.0)) * (1 - ignore_camera) + vec4(in_position.xyz , 1.0) * ignore_camera;
	//fragment.pos = (model * vec4(in_position.xyz, 1.0)).xyz;
	fragment.pos = vec3(camera.pos);
}