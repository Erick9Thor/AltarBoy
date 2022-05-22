#version 460 core

out vec4 fragment_color;

in vec2 texture_coords;

uniform int mode;
layout (binding = 0) uniform sampler2D g_specular_smoothness;
layout (binding = 1) uniform sampler2D g_diffuse;
layout (binding = 2) uniform sampler2D g_normal;
layout (binding = 3) uniform sampler2D g_position;

layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
    vec3 pos;
} camera;

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

#define PI 3.141597

struct AmbientLight
{
    vec4 color;
    float intensity;
};

struct DirLight
{
    vec4 direction;
    vec4 color;
    float intensity;
};

struct PointLight
{
    vec4 position;
    vec4 color;
    float intensity;
    float radius;
};

struct SpotLight
{
    vec4 position;
    vec4 direction;
    vec4 color;
    float inner;
    float outer;
    float intensity;
    float radius;
};

layout(std140, binding = 2) uniform Lights
{
    AmbientLight ambient;
    DirLight directional;
    PointLight points[MAX_POINT_LIGHTS];
    SpotLight spots[MAX_SPOT_LIGHTS];
    uint n_points;
    uint n_spots;
} lights;

// TODO: Add lights and stuff.
vec3 SchlickFresnel(const vec3 f0, float half_vector)
{
    return f0 + (vec3(1.0) - f0) * pow(1.0 - half_vector, 5.0);
}

float GGX(const vec3 normal, const vec3 halfway_dir, float roughness)
{
    float a2 = pow(roughness, 2.0);
    float NdotH = max(dot(normal, halfway_dir), 0.00001);
    float NdotH2 = pow(NdotH, 2.0);
    
    float denom = PI * pow((NdotH2 * (a2 - 1.0) + 1.0), 2.0);

    return  a2 / denom;
}

float SmithVisibilityFunction(const vec3 normal, const vec3 view_dir, const vec3 light_dir, float roughness)
{
    float NdotV = max(dot(normal, view_dir), 0.00001);
    float NdotL = max(dot(normal, light_dir), 0.00001);

    float denom = NdotL * (NdotV * (1 - roughness) + roughness) + NdotV * (NdotL * (1 - roughness) + roughness);
    return 0.5 / denom;
}

vec3 PBR(const vec3 normal, const vec3 view_dir, const vec3 light_dir,  const vec3 light_color,
         const vec3 diffuse_color, const vec3 F0, float attenuation, float smoothness)
{
    vec3 reflect_dir = reflect(-light_dir, normal);
    // Should equal cos theta
    float NdL = max(dot(normal, light_dir), 0.00001);
    // Phong specular strength
    float VdR =  max(dot(view_dir, reflect_dir), 0.00001);

    float roughness = pow((1.0 - smoothness), 2.0);

    vec3 halfway_dir = normalize(light_dir + view_dir);
    float VdotH = max(dot(view_dir, halfway_dir), 0.00001);

    float NDF =  GGX(normal, halfway_dir, roughness);
    float SVF = SmithVisibilityFunction(normal, view_dir, light_dir, roughness);
    vec3 fresnel = SchlickFresnel(F0, VdotH);
    
    vec3 part1 = diffuse_color * (vec3(1.0) - F0);
    vec3 part2 = (fresnel*SVF*NDF)/4;

    

    return light_color * (diffuse_color * (vec3(1.0) - F0) + 0.25*fresnel*SVF*NDF) * NdL * attenuation;
}

vec3 DirectionalPBR(const vec3 normal, const vec3 view_dir, const DirLight light,
                    const vec3 diffuse_color, const vec3 F0, float smoothness)
{
    vec3 L = normalize(-light.direction.xyz);
    
    float attenuation = 1.0;
    // Input dir goes from light to fragment, swap it for calculations
    return PBR(normal, view_dir, L, light.color.rgb, diffuse_color, F0, attenuation, smoothness) * light.intensity;
}

float Attenuation(float distance)
{
    return 1.0/(1.0+0.3*distance+0.3*(distance*distance));
}

float EpicAttenuation(float distance, float radius)
{
    return max(pow(max(1 - pow(distance/radius, 4), 0.0), 2.0), 0.0) / (distance * distance + 1);
}

vec3 PositionalPBR(const vec3 frag_pos, const vec3 normal, const vec3 view_dir, const PointLight light, 
                   const vec3 diffuse_color, const vec3 F0, float smoothness)
{
    vec3 L = light.position.xyz - frag_pos;
    float light_distance = length(L);
    L = normalize(L);

    float radius = 250.0;
    float attenuation = EpicAttenuation(light_distance, light.radius);
    return PBR(normal, view_dir, L, light.color.rgb, diffuse_color, F0, attenuation, smoothness) * light.intensity;
}

// TODO: Test more
float SpotAttenuation(const vec3 L, const vec3 cone_direction, float radius)
{
     // Light direction from light to outside
    float distance = dot(-L, cone_direction);
    return EpicAttenuation(distance, radius);
}

float Cone(const vec3 L, const vec3 cone_direction, float inner, float outer)
{
    // Light direction from light to fragment (Reverse than pbr)
    float C = dot(-L, cone_direction);
    float c_inner = cos(inner);
    if (C > c_inner) return 1.0;
    float c_outer = cos(outer);
    if (C > c_outer) return (C - c_outer)/(c_inner - c_outer);
    return 0.0;
}

vec3 SpotPBR(const vec3 frag_pos, const vec3 normal, const vec3 view_dir, const SpotLight light, 
             const vec3 diffuse_color, const vec3 F0, float smoothness)
{
    // Not the same as spot light direction (L)
    vec3 L = light.position.xyz - frag_pos;
    L = normalize(L);
    vec3 cone_direction = normalize(light.direction.xyz);

    float attenuation = SpotAttenuation(L, cone_direction, light.radius);
    float cone = Cone(L, cone_direction, light.inner, light.outer);
    return PBR(normal, view_dir, L, light.color.rgb, diffuse_color, F0, attenuation * cone, smoothness) * light.intensity;
}

void main()
{
    vec3  fragment_diffuse = (texture(g_diffuse, texture_coords)).rgb;
    vec3  fragment_specular = (texture(g_specular_smoothness, texture_coords)).rgb;
    float fragment_smoothness = (texture(g_specular_smoothness, texture_coords)).a;
    vec3  fragment_normal = (texture(g_normal, texture_coords)).rgb;
    vec3  fragment_position = (texture(g_position, texture_coords)).rgb;
    vec3 view_direction = normalize(camera.pos - fragment_position);

    if (mode == 0)
    {
        vec3 hdr_color = vec3(0.0);
        hdr_color += DirectionalPBR(fragment_normal, view_direction, lights.directional, fragment_diffuse, fragment_specular, fragment_smoothness);
        
        for(uint i=0; i<lights.n_points; ++i)
        {
            hdr_color +=  PositionalPBR(fragment_position, fragment_normal, view_direction, lights.points[i], fragment_diffuse, fragment_specular, fragment_smoothness);
        }

        for(uint i=0; i<lights.n_spots; ++i)
        {
            hdr_color +=  SpotPBR(fragment_position, fragment_normal, view_direction, lights.spots[i], fragment_diffuse, fragment_specular, fragment_smoothness);
            
        }

        hdr_color += fragment_diffuse * lights.ambient.color.rgb * lights.ambient.intensity;

        // Reinhard tone mapping
        vec3 ldr_color = hdr_color / (hdr_color + vec3(1.0));

        // Gamma correction & alpha from diffuse texture if it is transparent
        fragment_color = vec4(pow(ldr_color.rgb, vec3(1.0/2.2)), 1.0f);
    }

    // Debug modes:

    else if (mode == 1)
    {
        fragment_color = vec4(fragment_diffuse, 1.0f);
    }
    else if (mode == 2)
    {
        fragment_color = vec4(fragment_specular, 1.0f);
    }
    else if (mode == 3)
    {
        fragment_color = vec4(fragment_smoothness, 0.0f, fragment_smoothness, 1.0f);
    }
    else if (mode == 4)
    {
        fragment_color = vec4(fragment_normal, 1.0f);
    }
    else 
    {
        fragment_color = vec4(fragment_position, 1.0f);
    }
}