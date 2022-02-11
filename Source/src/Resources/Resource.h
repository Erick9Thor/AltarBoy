#pragma once

#include "Utils/UUID.h"


enum class ResourceType
{
	UNKNOWN,
	MATERIAL,
	TEXTURE,
	MODEL,
	MESH,
	SCENE,
	SHADER
};

class Resource
{
public:
	Resource(Hachiko::UID id, ResourceType type);
	virtual ~Resource();

	ResourceType GetType() const
	{
		return type;
	}

    Hachiko::UID GetID() const { return id; }

	unsigned int instances = 0;

private:
    Hachiko::UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
