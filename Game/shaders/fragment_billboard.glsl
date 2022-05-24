# version 460

in vec2 uv0;
uniform sampler2D diffuseMap;
uniform int hasDiffuseMap;

// Animation
uniform float x_factor;
uniform float y_factor;
uniform vec2 animation_index;

out vec4 outColor;

void main()
{	
    float u = uv0.x * x_factor;
    float v = uv0.y * y_factor;
    outColor = texture2D(diffuseMap,  vec2(u, v));
}