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
		
		update_status Update();

		void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);

		void CheckCameraControl();

		ComponentCamera* getMainCamera() { return main_camera; }

		// Camera actions
		void ZoomCamera(int zoom);

	private:
		ComponentCamera* main_camera = nullptr;
};

