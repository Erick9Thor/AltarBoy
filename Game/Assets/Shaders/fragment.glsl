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
    vec3 reflect_dir = reflect(-light_dir, normal);
    // Should equal cos theta
    float NdL = max(dot(normal, light_dir), 0.0001);
    // Phong specular strength
    float VdR =  max(dot(view_dir, reflect_dir), 0.0001);

    //vec3 halfway_dir = normalize(light_dir + view_dir);
    //float cos_theta = max(dot(view_dir, halfway_dir), 0.0);   
    //vec3 fresnel = SchlickFresnel(specular_color, cos_theta);

    vec3 fresnel = SchlickFresnel(specular_color, NdL);
    
    vec3 part1 = (diffuse_color * (vec3(1.0) - specular_color)) / PI;
    vec3 part2 = ((shininess + 2.0) / (2.0 * PI)) * fresnel * pow(VdR, shininess);

    return light_color * (part1 + part2) * NdL * attenuation;
}

// TODO: Implement and add shininess or smoothness
vec3 DirectionalPBR(const vec3 normal, const vec3 view_dir, const DirLight light,
                    const vec3 diffuse_color, const vec3 specular_color, float shininess)
{
    float attenuation = 1.0;
    // Input dir goes from light to fragment, swap it for calculations
    return PBR(normal, view_dir, normalize(-light.direction.xyz), light.color.rgb, diffuse_color, specular_color, shininess, attenuation);
}

float Attenuation(float distance)
{
    return 1.0/(1.0+0.3*distance+0.3*(distance*distance));
}

float EpicAttenuation(float distance)
{
    float radius = 1000.0;
    return max(pow(max(1 - pow(distance/radius, 4), 0.0), 2.0), 0.0) / (distance * distance + 1);
}

vec3 PositionalPBR(const vec3 frag_pos, const vec3 normal, const vec3 view_dir, const PointLight light, 
                   const vec3 diffuse_color, const vec3 specular_color, float shininess)
{
    vec3 light_direction = light.position.xyz - frag_pos;
    float distance = length(light_direction);
    light_direction = normalize(light_direction);

    float attenuation = 1.0; //sEpicAttenuation(distance);
    return PBR(normal, view_dir, light_direction, light.color.rgb, diffuse_color, specular_color, shininess, attenuation);
}

void main()
{
    vec3 norm = normalize(fragment.normal);
    vec3 view_dir = normalize(camera.pos - fragment.pos);
    vec3 diffuse_color = pow(texture(diffuse, fragment.tex_coord).rgb, vec3(2.2));

    vec3 plastic_specular = vec3(0.03);
    vec3 aluminum_specular = vec3(0.91, 0.92, 0.92);
    float shininess = 125.0;
    vec3 texture_color = DirectionalPBR(norm, view_dir, lights.directional, diffuse_color, plastic_specular, shininess);

    texture_color +=  PositionalPBR(fragment.pos, norm, view_dir, lights.point, diffuse_color, plastic_specular, shininess);

    texture_color += diffuse_color * 0.05;

    color = vec4(texture_color.rgb, texture(diffuse, fragment.tex_coord).a);

    // Gamma correction
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
}
