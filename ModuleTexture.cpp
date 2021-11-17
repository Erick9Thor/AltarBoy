#include "ModuleTexture.h"

#include "glew.h"

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

unsigned int ModuleTexture::GetTextureData(const char* path)
{
	ILuint name; // The image name to return.
	ilGenImages(1, &name); // Grab a new image name.
	ilBindImage(name);
	ilLoadImage(path);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluRotate(180.0f);

	return name;
}

update_status ModuleTexture::Update()
{

	return UPDATE_CONTINUE;
}

void ModuleTexture::CleanTexture(unsigned int& texture)
{
	ilDeleteImages(1, &texture);
}
