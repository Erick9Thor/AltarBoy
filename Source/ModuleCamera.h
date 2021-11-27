#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DrawGui();

	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void WindowResized(unsigned int screen_width, unsigned int screen_height);
	void checkCameraControl();

	float4x4 GetGLView() const;
	float4x4 GetView() const;

	float4x4 GetGLProjection() const;
	float4x4 GetProjection() const;

	void RefreshFov();

private:

	void LookAt();

	float3x3 rotation_matrix;

	Frustum frustum;
	float aspect_ratio;
	float3 position;

	float initial_vertical_fov = 45.0f;
	float vertical_fov = DEGTORAD * initial_vertical_fov;
	float near_plane_distance = 0.1f;
	float far_plane_distance = 200.0f;

	float camera_speed = 10.0f;
	float mouse_speed_rotation = 5.0f;
	float mouse_speed_movement = 2.5f;
};

