#pragma once
#include "../Module.h"

#include "../Math.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawTextureImGui(bool& showWindow);

private:
	unsigned int program;
	unsigned int vBuffer;
	unsigned int texture;

	float4x4 model;

	void SetTextureParameters();
};
