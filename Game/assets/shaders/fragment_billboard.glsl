# version 460

in vec2 uv0;
uniform sampler2D diffuseMap;
out vec4 outColor;

void main()
{	
    //outColor = texture2D(diffuseMap,  uv0);
    outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}