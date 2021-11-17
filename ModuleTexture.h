#pragma once
#include "Module.h"


class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status Update();

	bool CleanUp();

	unsigned int GetTextureData(const char* path);


	void CleanTexture(unsigned int& texture);

	unsigned int texture_id;
private:

	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int format;
};