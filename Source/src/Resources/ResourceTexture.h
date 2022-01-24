#pragma once

#include "Resource.h"


class ResourceTexture : public Resource
{
public:
	ResourceTexture(UID id);

	void Load() override;
	void Unload() override;
};
