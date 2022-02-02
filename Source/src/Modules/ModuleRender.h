#pragma once
#include "Module.h"
#include "../Globals.h"
#include "../UI/ImGuiUtils.h"

#include "MathGeoLib.h"

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
	update_status Update(const float delta) override;
	update_status PostUpdate(const float delta) override;
	bool CleanUp() override;

	inline unsigned int GetFrameBuffer() const { return frame_buffer; }
	inline unsigned int GetTextureId() const { return fb_texture; }

	void OptionsMenu();
	void PerformanceMenu();
	void FpsGraph();
	void AddFrame(const float delta);

	inline void* GetGLContext() const { return context; }
	inline const GpuData GetGpuData() const { return gpu; }
	inline const GlVersion GetGlVersion() { return gl; }

private:
	void GenerateFrameBuffer();
	void ResizeFrameBuffer(int heigth, int width);
	void ManageResolution(ComponentCamera* camera);
	void Draw(Scene* scene, ComponentCamera* camera, ComponentCamera* culling);

	void CreateContext();
	void SetGLOptions();
	void RetrieveLibVersions();
	void RetrieveGpuInfo();

	void* context;

	RenderList render_list;

	unsigned frame_buffer = 0;
	unsigned depth_stencil_buffer = 0;
	unsigned fb_texture = 0;	
	unsigned fb_height = 0;
	unsigned fb_width = 0;

	float4 clear_color;
	bool draw_skybox = true;
	bool outline_selection = true;

	GpuData gpu;
	GlVersion gl;
	int vram_free;

	static const unsigned n_bins = 50;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	float current_fps = 0.0f;
	float current_ms = 0.0f;
};
