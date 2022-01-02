#pragma once
#include "Module.h"

#include "../Program.h"

#include "MathGeoLib.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram() override;

	bool Init() override;
	bool CleanUp() override;

	Program* GetMainProgram() const { return main_program; }
	Program* GetSkyboxProgram() const { return skybox_program; }

	void OptionsMenu();

private:
	char* LoadShaderSource(const char* shader_file_name);
	unsigned int CompileShader(unsigned type, const char* source);
	void CompileShaders(const char* vtx_shader_path, const char* frg_shader_path, unsigned& vtx_shader, unsigned& frg_shader);
	Program* CreateProgram(const char* vtx_shader_path, const char* frg_shader_path);

	Program* CreateMainProgram();
	Program* CreateSkyboxProgram();

	Program* main_program;
	Program* skybox_program;

	struct Light
	{
		float3 position;
		float3 direction;
		float3 color;
		float ambient_strength;
		bool directional;
	};

	Light light;
};
