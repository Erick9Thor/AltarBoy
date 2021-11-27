#pragma once
#include "Module.h"

#include "il.h"
#include "ilu.h"

class ModuleTexture : public Module
{
	public:
		ModuleTexture();
		~ModuleTexture();

		bool Init();

		unsigned int GetTextureData(const char* path);
		void CleanTexture(unsigned int& texture);
	
		ILuint texture_id;
};