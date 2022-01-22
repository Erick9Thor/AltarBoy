#pragma once
#include "Module.h"

#include "../Program.h"

#include "MathGeoLib.h"
#include <vector>

class ComponentCamera;
class ComponentPointLight;

class ModuleProgram : public Module
{
public:

	enum UBOPoints
	{
		p_camera = 0,
		p_lights,
		n_ubo_points,
	};

	ModuleProgram();
	~ModuleProgram() override;

	bool Init() override;
	bool CleanUp() override;

	Program* GetMainProgram() const { return main_program; }
	Program* GetSkyboxProgram() const { return skybox_program; }

	void UpdateCamera(ComponentCamera* camera);
	void UpdateLights(std::vector<ComponentPointLight*>& point_lights);

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

	// Assume the shader already manages its binding points
	void CreateUBO(UBOPoints binding_point, unsigned size);
	void UpdateUBO(UBOPoints binding_point, unsigned size, void* data, unsigned offset = 0);
	void CreateCameraUBO(); 
	void CreateLightsUBO();

	struct Camera
	{
		float4x4 view = float4x4::identity;
		float4x4 proj = float4x4::identity;
		float3 pos = float3::zero;
	};

	unsigned ubos[UBOPoints::n_ubo_points];

	// Use float4 to prevent padding
	struct AmbientLight
	{
		float4 color = float4::zero;
	};

	struct DirLight
	{
		float4 direction = float4::zero;
		float4 color = float4::zero;
	};

	struct PointLight
	{
		float4 position = float4::zero;
		float4 color = float4::zero;
	};

	struct Lights
	{
		AmbientLight ambient;
		DirLight directional;
		PointLight point;
	};

	struct Light
	{
		float3 position;
		float3 direction;
		float3 color;
	};

	Light light;
};
