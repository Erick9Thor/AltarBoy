#pragma once
#include "Resource.h"

class ResourceScene : public Resource
{
	ResourceScene(UID id);

	void Load() override {};
	void Unload() override {};
};
