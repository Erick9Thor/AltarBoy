#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawGui();

	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void CheckCameraControl();

	// TODO: Make GL standard
	float4x4 GetGLView() const;
	float4x4 GetView() const;

	float4x4 GetGLProjection() const;
	float4x4 GetProjection() const;

	void RefreshFov();

	void OrbitCamera(float motion_x, float motion_y);

	void FocusOnModel(Model* model);

private:

	void UpdateCamera();

	float3 reference_point = float3::zero;
	float3x3 rotation_matrix;

	float initial_vertical_fov = 45.0f;
	float vertical_fov = DEGTORAD * initial_vertical_fov;

	Frustum frustum;
	float aspect_ratio;
	float3 position;

	float camera_speed = 10.0f;
	float mouse_speed_rotation = 5.0f;
	float mouse_speed_movement = 2.5f;
};

