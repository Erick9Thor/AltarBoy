# version 460

layout(location=0) in vec2 input_texture_coords;

out vec2 texture_coords;

void main()
{
    texture_coords = input_texture_coords;
}