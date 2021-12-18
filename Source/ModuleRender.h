#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
	public:
		ModuleRender();
		~ModuleRender();

		bool Init();
		update_status PreUpdate();
		update_status Update();
		update_status PostUpdate();

		void GenerateFrameBuffer();
		
		bool CleanUp();
		void WindowResized(unsigned int width, unsigned int height);

		bool initGlew();
		bool initializeOpenGLviaSDL();

		void* context;

	private:
		unsigned int frame_buffer;
		unsigned int fb_texture;
		unsigned int depth_stencil_buffer;

		float2 viewportPanelSize = float2(0.0f, 0.0f);
};
