#pragma once
#include "Module.h"
#include "../Globals.h"

#include "MathGeoLib.h"

class ComponentCamera;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() override;

	bool Init() override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	void OnResize(unsigned int screen_width, unsigned int screen_height);

	void Controller(const float delta);
	void MovementController(const float delta);

	ComponentCamera* GetMainCamera() { return main_camera; }

	// Camera actions
	void Zoom(float zoom);
	void Orbit(float motion_x, float motion_y);
	void FocusOnModel(const float3& target, float distance);
	void Rotate(float motion_x, float motion_y);

private:
	GameObject* main_camera_game_object = nullptr; // TODO: This name may be too long for Eric or Bernat's taste.
	ComponentCamera* main_camera = nullptr;
};
