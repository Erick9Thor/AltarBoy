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

Texture ModuleTexture::LoadTexture(const char* path)
{
	Texture texture;
	texture.path = path;
	unsigned int img_id = GetTextureData(path);

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	ilDeleteImages(1, &img_id);
	return texture;
}

void ModuleTexture::CleanTexture(unsigned int& texture)
{
	LOG("[M_Texture] Cleaning textures");
	ilDeleteImages(1, &texture);
}

void ModuleTexture::DrawGui() {

}