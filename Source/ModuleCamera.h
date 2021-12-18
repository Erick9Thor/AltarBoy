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
		~ModuleCamera() override;

		bool Init() override;
		update_status Update(const float delta) override;

		void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);

		void Controller(const float delta);
		void MovementController(const float delta);

		ComponentCamera* getMainCamera() { return main_camera; }

		// Camera actions
		void ZoomCamera(int zoom);
		void OrbitCamera(float motion_x, float motion_y);
		void FocusCameraOnTarget(const float3& target, float distance);
		void RotationCamera(float motion_x, float motion_y);

	private:
		ComponentCamera* main_camera = nullptr;
};

