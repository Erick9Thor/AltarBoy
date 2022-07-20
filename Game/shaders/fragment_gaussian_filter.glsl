#version 460

out vec4 fragment_color;
in vec2 texture_coords;

uniform vec3 blur_scale;
uniform int blur_pixels;
uniform float blur_sigma;
layout (binding = 0) uniform sampler2D input_texture; 

const float pi = 3.14159265f;

void main()
{
    // vec4 color = vec4(0.0);

    // color += texture(input_texture, vec2(-3.0) * blur_scale.xy + texture_coords) * (1.0/64.0);
    // color += texture(input_texture, vec2(-2.0) * blur_scale.xy + texture_coords) * (6.0/64.0);
    // color += texture(input_texture, vec2(-1.0) * blur_scale.xy + texture_coords) * (15.0/64.0);
    // color += texture(input_texture, vec2(0.0) * blur_scale.xy + texture_coords)  * (20.0/64.0);
    // color += texture(input_texture, vec2(1.0) * blur_scale.xy + texture_coords)  * (15.0/64.0);
    // color += texture(input_texture, vec2(2.0) * blur_scale.xy + texture_coords)  * (6.0/64.0);
    // color += texture(input_texture, vec2(3.0) * blur_scale.xy + texture_coords)  * (1.0/64.0);


    // const int num_indices = 21;
    // // We assume that num_indices is an odd number:
    // const int indices_half = (num_indices - 1) / 2;


    // float coefficients[21] = 
    // {
    //     0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
    //     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
    //     0.14107424,
    //     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
    //     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337
    // };


    // for (int index = 0; index < num_indices; ++index)
    // {
    //     color += texture(input_texture, vec2((index - indices_half)) * blur_scale.xy + texture_coords) * coefficients[index];
    // }

    // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
    vec3 incremental_gaussian;
    incremental_gaussian.x = 1.0 / (sqrt(2.0 * pi) * blur_sigma);
    incremental_gaussian.y = exp(-0.5 / (blur_sigma * blur_sigma));
    incremental_gaussian.z = incremental_gaussian.y * incremental_gaussian.y;

    vec4 average_value = vec4(0.0);
    float coefficient_sum = 0.0;

    // Take the central sample first...
    average_value += texture2D(input_texture, texture_coords) * incremental_gaussian.x;
    coefficient_sum += incremental_gaussian.x;
    incremental_gaussian.xy *= incremental_gaussian.yz;

    // Go through the remaining 8 vertical samples (4 on each side of the center)
    for (int i = 1; i <= blur_pixels; i++) 
    { 
        average_value += texture2D(input_texture, texture_coords - i * blur_scale.xy) * incremental_gaussian.x;         
        average_value += texture2D(input_texture, texture_coords + i * blur_scale.xy) * incremental_gaussian.x;         
        coefficient_sum += 2 * incremental_gaussian.x;
        incremental_gaussian.xy *= incremental_gaussian.yz;
    }


    fragment_color = average_value / coefficient_sum;
}
 