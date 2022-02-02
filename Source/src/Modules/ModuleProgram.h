#pragma once
#include "Module.h"

#include "../Program.h"

#include "MathGeoLib.h"
#include <vector>
#include "../UI/ImGuiUtils.h"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

class ComponentCamera;
class ComponentDirLight;
class ComponentPointLight;
class ComponentSpotLight;
class ComponentMaterial;

class ModuleProgram : public Module
{
public:

	enum UBOPoints
	{
		p_camera = 0,
		p_material,
		p_lights,
		n_ubo_points,
	};

	enum TextureSlots
	{
		t_diffuse = 0,
		t_specular,
		n_texture_slots,
	};

	ModuleProgram();
	~ModuleProgram() override;

	bool Init() override;
	bool CleanUp() override;

	Program* GetMainProgram() const { return main_program; }
	Program* GetSkyboxProgram() const { return skybox_program; }
	Program* GetStencilProgram() const { return stencil_program; }

	void UpdateCamera(const ComponentCamera* camera);
	void UpdateMaterial(const ComponentMaterial* material_comp);
	void UpdateLights(const ComponentDirLight* dir_light, const std::vector<ComponentPointLight*>& point_lights, const std::vector<ComponentSpotLight*>& spot_lights);

	void OptionsMenu();

private:
	char* LoadShaderSource(const char* shader_file_name);
	unsigned int CompileShader(unsigned type, const char* source);
	void CompileShaders(const char* vtx_shader_path, const char* frg_shader_path, unsigned& vtx_shader, unsigned& frg_shader);
	Program* CreateProgram(const char* vtx_shader_path, const char* frg_shader_path);

	Program* CreateMainProgram();
	Program* CreateSkyboxProgram();
	Program* CreateStencilProgram();

	Program* main_program;
	Program* skybox_program;
	Program* stencil_program;

	// Assume the shader already manages its binding points
	void CreateUBO(UBOPoints binding_point, unsigned size);
	void UpdateUBO(UBOPoints binding_point, unsigned size, void* data, unsigned offset = 0);
	void CreateCameraUBO(); 
	void CreateMaterialUBO();
	void CreateLightsUBO();

	unsigned ubos[UBOPoints::n_ubo_points];

	struct Camera
	{
		float4x4 view = float4x4::identity;
		float4x4 proj = float4x4::identity;
		float3 pos = float3::zero;
	};

	struct MaterialData
	{
		float4 diffuse_color;
		float4 specular_color;
		unsigned diffuse_flag;
		unsigned specular_flag;
		float shininess;		
	};
	

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
	
	AmbientLight ambient_light;
	float ambient_strength = 0.05f;
};
