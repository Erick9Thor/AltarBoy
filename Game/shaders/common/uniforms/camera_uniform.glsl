layout(std140, row_major, binding = 0) uniform Camera
{
    mat4 view;
    mat4 proj;
#ifndef CAMERA_WITHOUT_POSITION
    vec3 pos;
#endif
} camera;