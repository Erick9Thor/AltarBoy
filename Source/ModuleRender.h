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
		~ModuleRender() override;

		bool Init() override;
		update_status PreUpdate(const float delta) override;
		update_status Update(const float delta) override;
		update_status PostUpdate(const float delta) override;
		bool CleanUp() override;

		void WindowResized(unsigned int width, unsigned int height);

		bool initGlew();
		bool initializeOpenGLviaSDL();

		void* context;
};
