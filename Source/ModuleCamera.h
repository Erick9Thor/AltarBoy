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

		ComponentCamera* getViewPortCamera() { return main_camera; }

		// Camera actions
		void Zoom(int zoom);
		void Orbit(float motion_x, float motion_y);
		void LookAt(const float3& target, float distance);
		void Rotate(float motion_x, float motion_y);

	private:
		ComponentCamera* main_camera = nullptr;
};

