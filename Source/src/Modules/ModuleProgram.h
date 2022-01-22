#pragma once
#include "Module.h"

#include "../Program.h"

#include "MathGeoLib.h"
#include <vector>

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

class ComponentCamera;
class ComponentDirLight;
class ComponentPointLight;
class ComponentSpotLight;

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
	void UpdateLights(ComponentDirLight* dir_light, std::vector<ComponentPointLight*>& point_lights, std::vector<ComponentSpotLight*>& spot_lights);

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
		float4 color = float4::one;
		float intensity = 0.05f;
		float padding[3];
	};

	struct DirLight
	{
		float4 direction = float4::zero;
		float4 color = float4::zero;
		float intensity;
		float padding[3];
	};

	struct PointLight
	{
		float4 position = float4::zero;
		float4 color = float4::zero;
		float intensity;
		float radius;
		float padding[2];
	};

	struct SpotLight
	{
		float4 position = float4::zero;
		float4 direction = float4::zero;
		float4 color = float4::zero;
		float inner;
		float outer;
		float intensity;
		float radius;
	};

	struct Lights
	{
		AmbientLight ambient;
		DirLight directional;
		PointLight points[MAX_POINT_LIGHTS];
		SpotLight spots[MAX_SPOT_LIGHTS];
		unsigned int n_points;
		unsigned int n_spots;
	};

	struct Light
	{
		float3 position;
		float3 direction;
		float3 color;
	};

	AmbientLight ambient_light;
	float ambient_strength = 0.05f;
};
