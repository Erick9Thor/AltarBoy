# version 330
out vec4 color;

in vec2 tex_coord;

uniform sampler2D texture;

void main()
{
    color = texture2D(texture, tex_coord);
}
