# version 460

// Input
in vec2 uv0;

// Texture
uniform sampler2D diffuseMap;
uniform int has_texture;

// Animation
uniform int flip_x;
uniform int flip_y;
uniform float x_factor;
uniform float y_factor;
uniform vec2 animation_index;
uniform float current_frame;

// Color Over Lifetime
uniform vec4 input_color;

// Output
out vec4 output_color;

void main()
{	
    // Flip UVs
	float u = flip_x * (1 - uv0.x) + (1 - flip_x) * uv0.x;
	float v = flip_y * (1 - uv0.y) + (1 - flip_y) * uv0.y;

    // Blend tiles
    //float X = trunc(mod(current_frame, (1/x_factor)));
	//float Y = trunc(current_frame * y_factor);
	//Y = ((1/y_factor) - 1) - Y;
	//u = mix(X, X + 1, u) * x_factor;
	//v = mix(Y, Y + 1, v) * y_factor;

    u = (u + animation_index.x) * x_factor;
    v = (v + animation_index.y) * y_factor;
    output_color = has_texture * texture2D(diffuseMap,  vec2(u, v)) * input_color + (1 - has_texture) * input_color;
}