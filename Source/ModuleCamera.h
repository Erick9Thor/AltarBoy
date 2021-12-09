#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

#include "MathGeoLib.h"

class ComponentCamera;

class ModuleCamera : public Module
{
	public:
		ModuleCamera();
		~ModuleCamera();

		bool Init() override;
		update_status Update();

		void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);

		void CheckCameraControl();

		ComponentCamera* getMainCamera() { return main_camera; }

		// Camera actions
		void ZoomCamera(int zoom);
		void OrbitCamera(float motion_x, float motion_y);
		void MoveCamera(float motion_x, float motion_y);
		void FocusCameraOnTarget(const float3& target, float distance);
		void RotationCamera(float motion_x, float motion_y);

	private:
		ComponentCamera* main_camera = nullptr;
};

