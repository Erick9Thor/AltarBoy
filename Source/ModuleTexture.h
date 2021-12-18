#pragma once
#include "Module.h"
#include "Texture.h"

#include "il.h"
#include "ilu.h"

#include <string>

class ModuleTexture : public Module
{
	public:
		ModuleTexture();
		~ModuleTexture() override;

		bool Init() override;
		bool CleanUp() override;

		unsigned int GetTextureData(const char* path);

		// TODO: Implemet?
		void CleanTexture(unsigned int& texture);

		void DrawGui();
	
		ILuint texture_id;
};