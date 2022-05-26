# version 460

in vec2 uv0;
uniform sampler2D diffuseMap;
uniform int has_diffuse_map;

// Animation

uniform float x_factor;
uniform float y_factor;
uniform vec2 animation_index;

// Color Over Lifetime
uniform vec4 input_color;

out vec4 output_color;

void main()
{	
    if (has_diffuse_map == 1)
    {
        float u = (uv0.x + animation_index.x) * x_factor;
        float v = (uv0.y + animation_index.y) * y_factor;
        output_color = texture2D(diffuseMap,  vec2(u, v));
    }
    else
    {
        output_color = input_color;
    }
}