#include "ModuleTexture.h"

#include "glew.h"

#include "il.h"
#include "ilu.h"

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
	LOG("[ModuleTexture] Init Image library using DevIL lib version %d", IL_VERSION);
	return true;
}

unsigned int ModuleTexture::GetTextureData(const char* path)
{
	LOG("[ModuleTexture] Loading texure from: %s", path);

	ILuint texture_id; 
	ilGenImages(1, &texture_id); 
	ilBindImage(texture_id);

	if (!ilLoadImage(path)) {
		LOG("[ModuleTexture] Error loading texture from: %s", path);
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	ILinfo textureInfo;
	iluGetImageInfo(&textureInfo);
	if (textureInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();

	return texture_id;
}

void ModuleTexture::CleanTexture(unsigned int& texture)
{
	LOG("[ModuleTexture] Cleaning textures");
	ilDeleteImages(1, &texture);
}
