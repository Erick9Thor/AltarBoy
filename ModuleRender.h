#pragma once
#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__

#include "Module.h"
#include "Globals.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	char* LoadShaderSource(const char* shader_file_name);
	unsigned int CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	void RenderVBO(unsigned vbo, unsigned program);

	unsigned CreateTriangleVBO();

	bool Init();
	bool initGlew();
	bool initializeOpenGLviaSDL();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	void* context;

private:
	unsigned program;
	unsigned vbo;
};

#endif