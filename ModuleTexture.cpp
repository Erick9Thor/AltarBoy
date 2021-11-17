#include "ModuleTexture.h"

#include "IL/il.h"
#include "IL/ilu.h"

ModuleTexture::ModuleTexture()
{
	ilInit();
}

ModuleTexture::~ModuleTexture()
{
	ilShutDown();
}

bool ModuleTexture::Init()
{
	LOG("Init Image library using DevIL lib version %d", IL_VERSION);

	return true;
}

bool ModuleTexture::CleanUp()
{
	LOG("Freeing textures and Image library");

	return true;
}

void ModuleTexture::GetTextureData(unsigned int& texture, const char* source, int& width, int& height, byte*& data)
{
	ilGenImages(1, &texture);
	ilBindImage(texture);
	ilLoadImage(source);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	data = ilGetData();

	ILinfo textureInfo;
	iluGetImageInfo(&textureInfo);
	if (textureInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();

	this->width = textureInfo.Width;
	this->height = textureInfo.Height;
	this->depth = textureInfo.Depth;
	this->format = textureInfo.Format;
	//IL_COLOUR_INDEX, IL_RGB, IL_RGBA, IL_BGR, IL_BGRA, IL_LUMINANCE
}

void ModuleTexture::CleanTexture(unsigned int& texture)
{
	ilDeleteImages(1, &texture);
}

void ModuleTexture::GetLastTextureInfo(unsigned int& width, unsigned int& height, unsigned int& depth, unsigned int& format) const
{
	width = this->width;
	height = this->height;
	depth = this->depth;
	format = this->format;
}
