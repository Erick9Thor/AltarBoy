#pragma once
#include "Module.h"

#include "il.h"
#include "glew.h"

#include <string>

struct Texture
{
	bool loaded = false;
	unsigned id = 0;
	std::string path;
	unsigned width = 0;
	unsigned height = 0;
};

struct TextureCube
{
	bool loaded = false;
	unsigned id;
	unsigned widths[6];
	unsigned heighths[6];
};


class ModuleTexture : public Module
{

public:
	ModuleTexture();
	~ModuleTexture() override;

	bool Init() override;
	bool CleanUp() override;

	Texture Load(const char* path, bool flip = true);
	void Unload(Texture& texture);
	TextureCube LoadCubeMap(const char* paths[6]);
	void Bind(unsigned id, unsigned slot);
	void Unbind(unsigned slot);

	inline const short GetDevilVersion() const { return devil_version; }

	void OptionsMenu();

private:
	const short devil_version = IL_VERSION;
	unsigned int LoadImg(const char* path, bool flip = true);
	void DeleteImg(unsigned& img_id);
};
