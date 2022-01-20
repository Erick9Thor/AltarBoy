# version 440

#define PI 3.141597

struct AmbientLight
{
    vec4 color;
};

struct DirLight
{
    vec4 direction;
    vec4 color;
};

struct PointLight
{
    vec4 position;
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
    PointLight point;
} lights;

uniform sampler2D diffuse;

uniform float ambient_strength;

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

vec3 SchlickFresnel(const vec3 f0, float cos_theta)
{
    return f0 + (vec3(1.0) - f0) * pow(1.0 - cos_theta, 5.0);
}

vec3 PBR(const vec3 normal, const vec3 view_dir, const vec3 light_dir,  const vec3 light_color,
         const vec3 diffuse_color, const vec3 specular_color, float shininess, float attenuation)
{
    float NdL = max(dot(normal, light_dir), 0.0);
    float VdR =  max(dot(view_dir, reflect(light_dir, normal)), 0.0);

    vec3 halfway_dir = normalize(light_dir + view_dir);
    float cos_theta = max(dot(view_dir, halfway_dir), 0.0);

    // NDL = cosLi;
    //float cosLi = max(dot(normal, light_dir), 0.0);
    //float cosLh = max(dot(normal, halfway_dir), 0.0);

    vec3 fresnel = SchlickFresnel(specular_color, cos_theta);
    
    return (diffuse_color * (vec3(1.0) - specular_color) / PI + (shininess+2)/(2*PI) * fresnel * pow(VdR, shininess)) * light_color * NdL * attenuation;

}

// TODO: Implement and add shininess or smoothness
vec3 DirectionalPBR(const vec3 normal, const vec3 view_dir, const DirLight light,
                    const vec3 diffuse_color, const vec3 specular_color)
{
    float shininess = 50.0;
    float attenuation = 1.0;
    // Not sure if direction should be normalized
    return PBR(normal, view_dir, normalize(-light.direction.xyz), light.color.rgb, diffuse_color, specular_color, shininess, attenuation);
}

void main()
{
    vec3 norm = normalize(fragment.normal);
    vec3 view_dir = normalize(camera.pos - fragment.pos);
    vec3 texture_color = texture(diffuse, fragment.tex_coord).rgb;

    vec3 plastic_specular = vec3(0.03);
    texture_color = DirectionalPBR(norm, view_dir, lights.directional, texture_color, plastic_specular);

    color = vec4(texture_color.rgb, texture(diffuse, fragment.tex_coord).a);

    // Gamma correction
    //color.rgb = pow(color.rgb, vec3(1.0/2.2));
}
