# version 460

in vec2 uv0;
uniform sampler2D diffuseMap;
uniform int hasDiffuseMap;
out vec4 outColor;

void main()
{	
    outColor = texture2D(diffuseMap,  uv0);
}