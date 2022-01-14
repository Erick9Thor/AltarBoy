#pragma once

#include "../Utils/UID.h"


enum class ResourceType
{
	UNKNOWN,
	MATERIAL,
	MESH,
	SCENE,
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

	virtual void Load();
	virtual void Unload();

private:
	UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
