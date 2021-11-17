#pragma once
#include "Module.h"

#include "Math/float4x4.h"
#include "Math/float3x3.h"
#include "Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	void WindowResized(unsigned width, unsigned height);

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

	void SetPlaneDistances(float nearDistance, float farDistance);

	float4x4 view, proj;
	float3x3 rotationMatrix;

private:
	void ViewProjectionMatrix();

	Frustum frustum;
	int aspect;
	float3 position;
};