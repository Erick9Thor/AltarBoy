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
	
	void SetPosition(const float3& position);
	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void SetHorizontalFov(float fov_deg);
	void LookAt(const float3& position);

	void RefreshFov();
	void WindowResized(unsigned int screen_width, unsigned int screen_height);

	void checkCameraControl();

	bool locked;
	float3 position;

	float4x4 GetGLView() const;
	float4x4 GetView() const;

	float4x4 GetGLProjection() const;
	float4x4 GetProjection() const;

private:
	Frustum frustum;
	float aspect_ratio;
	float horizontal_fov;
	float3 look_position;
};

