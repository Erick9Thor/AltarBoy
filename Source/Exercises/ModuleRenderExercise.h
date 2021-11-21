#pragma once
#include "../Module.h"

#include "../Math.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

private:
	unsigned int square_vbo;
	unsigned int square_ebo;
	unsigned int shader_id;
	
	float4x4 model;
};
