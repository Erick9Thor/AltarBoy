#include "ModuleTexture.h"

#include "ilu.h"
#include "imgui.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	ilInit();
	return true;
}

bool ModuleTexture::CleanUp()
{
	ilShutDown();
	return true;
}

Texture ModuleTexture::Load(const char* path)
{
	Texture texture;
	texture.path = path;
	unsigned int img_id = LoadImg(path);

	if (img_id != 0)
	{
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), texture.width = ilGetInteger(IL_IMAGE_WIDTH), texture.height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		DeleteImg(img_id);
		texture.loaded = true; // False by default
	}
	return texture;
}

void ModuleTexture::Bind(unsigned id, unsigned slot)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void ModuleTexture::Unbind(unsigned slot)
{
	glDisable(GL_TEXTURE_2D);
}

void SetOption(unsigned option, unsigned value)
{
	glTexParameteri(GL_TEXTURE_2D, option, value);
}

void ModuleTexture::OptionsMenu()
{
	const char* labels_mag[] = {"Linear", "Nearest"};
	const unsigned values_mag[] = {GL_LINEAR, GL_NEAREST};
	const char* labels_min[] = {"Nearest", "Linear", "Nearest Mipmaps Nearest Criteria", "Nearest Mipmap Linear Criteria", "Linear Mipmaps (Two Closest) Nearest Criteria", "Linear Mipmaps (Two Closest) Linear Criteria"};
	const unsigned values_min[] = {GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR};
	static int mag_filter = 0; // Default is GL_LINEAR
	static int min_filter = 3; // Default is GL_NEAREST_MIPMAP_LINEAR

	if (ImGui::Combo("Mag Filter", &mag_filter, labels_mag, IM_ARRAYSIZE(labels_mag)))
		SetOption(GL_TEXTURE_MAG_FILTER, values_mag[mag_filter]);
	if (ImGui::Combo("Min Filter", &min_filter, labels_min, IM_ARRAYSIZE(labels_min)))
		SetOption(GL_TEXTURE_MIN_FILTER, values_min[min_filter]);
}

unsigned int ModuleTexture::LoadImg(const char* path)
{
	ILuint img_id; // The image name to return.
	ilGenImages(1, &img_id); // Grab a new image name.
	ilBindImage(img_id);
	if (!ilLoadImage(path))
		return 0;
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluFlipImage();

	return img_id;
}

void ModuleTexture::DeleteImg(unsigned& img_id)
{
	ilDeleteImages(1, &img_id);
}
