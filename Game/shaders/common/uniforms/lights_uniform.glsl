#ifndef _LIGHTS_UNIFORM_INCLUDE_
#define _LIGHTS_UNIFORM_INCLUDE_

layout(std140, binding = 2) uniform Lights
{
    AmbientLight ambient;
    DirLight directional;
    PointLight points[MAX_POINT_LIGHTS];
    SpotLight spots[MAX_SPOT_LIGHTS];
    uint n_points;
    uint n_spots;
} lights;

#endif
