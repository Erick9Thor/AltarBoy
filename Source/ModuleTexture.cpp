#include "ModuleTexture.h"

#include "glew.h"

#include "il.h"
#include "ilu.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	LOG("[M_Texture] Init Image library using DevIL lib version %d", IL_VERSION);
	ilInit();
	return true;
}

bool ModuleTexture::CleanUp()
{
	ilShutDown();
	return true;
}

unsigned int ModuleTexture::GetTextureData(const char* path)
{
	LOG("[M_Texture] Loading texure from: %s", path);

	ILuint texture_id; 
	ilGenImages(1, &texture_id); 
	ilBindImage(texture_id);

	if (!ilLoadImage(path)) {
		LOG("[M_Texture] Error loading texture from: %s", path);
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	ILinfo textureInfo;
	iluGetImageInfo(&textureInfo);
	if (textureInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();

	return texture_id;
}

void ModuleTexture::DrawGui() {

}