# version 440

struct AmbientLight
{
    vec4 color;
};

struct DirLight
{
    vec4 dir;
    vec4 color;
};

struct PointLight
{
    vec4 pos;
    vec4 color;
};

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
    vec3 pos;
} camera;

layout(std140, binding = 1) uniform Lights
{
    AmbientLight ambient;
    DirLight directional;
    //PointLight point; TODO: Set to array
} lights;


uniform sampler2D diffuse;

uniform vec3 light_position;
uniform vec3 ligh_direction;
uniform vec3 light_color;
uniform float ambient_strength;
uniform bool is_directional;

// Inputs
struct VertexData
{
    vec3 normal;
    vec3 pos;
    vec2 tex_coord;
};
in VertexData fragment;

// Outputs
out vec4 color;

void main()
{
    float diffuse_strength = 0.0;
    vec3 norm = normalize(fragment.normal);

    if (!is_directional) {
        diffuse_strength = max(dot(norm, normalize(light_position - fragment.pos)), 0.0);
    } else {
        diffuse_strength = max(dot(norm, normalize(ligh_direction)), 0.0);
    }

    vec3 texture_color = texture(diffuse, fragment.tex_coord).xyz;
    texture_color = (ambient_strength + diffuse_strength) * light_color * texture_color;
    color = vec4(texture_color.xyz, texture(diffuse, fragment.tex_coord).w);
}
