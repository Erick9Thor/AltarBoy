#include "Texture.h"
#include "Application.h"
#include "ModuleTexture.h"

#include "glew.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Texture::Texture(const char* path)
{
	texture_path = path;
	texture_id = App->texture->GetTextureData(path);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	glGenerateTextureMipmap(texture_id);

	ilDeleteImages(1, &texture_id);
}


Texture::~Texture() {
	LOG("[Texture] Cleaning textures");
	ilDeleteImages(1, &texture_id);
}