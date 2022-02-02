#pragma once

#include "Modules/ModuleTexture.h"

#include "MathGeoLib.h"

class ComponentCamera;

class Skybox
{
public:
	Skybox();
	~Skybox();
	void Draw(ComponentCamera* camera);

private:
	TextureCube texture;
	unsigned vao;
	unsigned vbo;
};
