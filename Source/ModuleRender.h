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
		bool CleanUp();
		void WindowResized(unsigned int width, unsigned int height);

		bool initGlew();
		bool initializeOpenGLviaSDL();

		void* context;

		Model* house = nullptr;

	private:
		unsigned int texture_id;
};
