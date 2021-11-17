#pragma once
#include "Module.h"


class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();

	bool CleanUp();

	void GetTextureData(unsigned int& texture, const char* source, int& width, int& height, byte*& data);
	void CleanTexture(unsigned int& texture);

	void GetLastTextureInfo(unsigned int& width, unsigned int& height, unsigned int& depth, unsigned int& format) const;

private:
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int format;
};