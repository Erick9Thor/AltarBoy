#define MAX_POINT_LIGHTS 1200
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