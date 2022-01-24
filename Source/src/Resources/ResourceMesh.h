#pragma once
#include "Resource.h"


class ResourceMesh : public Resource
{
public:

	ResourceMesh(UID id);


	void Load() override;
	void Unload() override;
};
