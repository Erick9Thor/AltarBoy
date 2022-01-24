#pragma once
#include "Resource.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(UID id);

	void Load() override;
	void Unload() override;
};
