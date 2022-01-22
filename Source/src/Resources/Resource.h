#pragma once

#include "../Utils/UID.h"


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
	Resource(UID id, ResourceType type);
	virtual ~Resource();

	ResourceType GetType() const
	{
		return type;
	}

	UID getID() const { return id; }

private:
	UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
