#version 330

in vec2 uv0;

uniform sampler2D texture;

out vec4 color;

void main()
{
	color = texture2D(texture, uv0);
}
