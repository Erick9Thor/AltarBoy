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
		~ModuleTexture();

		bool Init();
		bool CleanUp();

		unsigned int GetTextureData(const char* path);
		void CleanTexture(unsigned int& texture);

		void DrawGui();
	
		ILuint texture_id;
};