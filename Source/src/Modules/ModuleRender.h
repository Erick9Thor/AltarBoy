#pragma once
#include "Module.h"
#include "../Globals.h"

#include "../Utils/Math.h"

#include "../RenderList.h"

#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class ComponentCamera;
class Scene;

struct GpuData
{
	unsigned char* name;
	unsigned char* brand;
	float vram_budget_mb;
};

struct GlVersion
{
	unsigned char* glew;
	unsigned char* opengl;
	unsigned char* glsl;
};

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender() override;

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	update_status Update(const float delta) override;
	update_status PostUpdate(const float delta) override;
	bool CleanUp() override;

	void WindowResized(unsigned width, unsigned height);

	void OptionsMenu();
	void PerformanceMenu(const float delta);
	void FpsGraph();
	void AddFrame(const float delta);

	inline void* GetGLContext() const
	{
		return context;
	}
	inline const GpuData GetGpuData() const
	{
		return gpu;
	}
	inline const GlVersion GetGlVersion()
	{
		return gl;
	}
	
	void Draw(Scene* scene, ComponentCamera* camera, ComponentCamera* culling);

private:
	void CreateContext();
	void SetGLOptions();
	void RetrieveLibVersions();
	void RetrieveGpuInfo();

	void* context;

	RenderList render_list;

	float4 clear_color;

	GpuData gpu;
	GlVersion gl;
	int vram_free;

	static const unsigned n_bins = 50;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	float current_fps = 0.0f;
	float current_ms = 0.0f;
};
