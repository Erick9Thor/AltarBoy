#version 460

out vec4 fragment_color;
in vec2 texture_coords;

uniform vec3 blur_scale;
layout (binding = 0) uniform sampler2D input_texture; 


void main()
{
    vec4 color = vec4(0.0);

    color += texture(input_texture, vec2(-3.0) * blur_scale.xy + texture_coords) * (1.0/64.0);
    color += texture(input_texture, vec2(-2.0) * blur_scale.xy + texture_coords) * (6.0/64.0);
    color += texture(input_texture, vec2(-1.0) * blur_scale.xy + texture_coords) * (15.0/64.0);
    color += texture(input_texture, vec2(0.0) * blur_scale.xy + texture_coords)  * (20.0/64.0);
    color += texture(input_texture, vec2(1.0) * blur_scale.xy + texture_coords)  * (15.0/64.0);
    color += texture(input_texture, vec2(2.0) * blur_scale.xy + texture_coords)  * (6.0/64.0);
    color += texture(input_texture, vec2(3.0) * blur_scale.xy + texture_coords)  * (1.0/64.0);

    fragment_color = color;
}
 