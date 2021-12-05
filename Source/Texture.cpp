#include "Texture.h"
#include "Application.h"
#include "ModuleTexture.h"

#include "glew.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Texture::Texture(const char* file_data, const char* path)
{
	LOG("[Texture] Loading texture");
	bool texture_loaded = false;

	string model_texture_path(file_data);
	string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);

	string default_path("Textures\\");

	if (texture_id = App->texture->GetTextureData((path + texture_file).c_str()) != 0) {
		LOG("[Texture] Texture found on: %s", path);
		texture_loaded = true;
	} 
	else if (texture_id = App->texture->GetTextureData((default_path + file_data).c_str()) != 0) {
		LOG("[Texture] Texture found on: %s", (default_path + file_data).c_str());
		texture_loaded = true;
	}

	if (texture_loaded) {
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData());

		glGenerateTextureMipmap(texture_id);
	}
	
	ilDeleteImages(1, &texture_id);
}

void Texture::DrawImGui()
{
	ImGui::Image((void*)(intptr_t)texture_id, ImVec2(150, 150));
}

Texture::~Texture() {
	LOG("[Texture] Cleaning textures");
	ilDeleteImages(1, &texture_id);
}