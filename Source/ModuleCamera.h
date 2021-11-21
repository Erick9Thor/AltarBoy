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

	float3x3 rotationMatrix;

	Frustum frustum;
	float aspect_ratio;
	float3 position;

	float initialVerticalFov = 45.0f;
	float verticalFov = DEGTORAD * initialVerticalFov;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 200.0f;

	float CameraSpeed = 10.0f;
	float mouseSpeedForRotation = 5.0f;
	float mouseSpeedForMovement = 2.5f;
};

