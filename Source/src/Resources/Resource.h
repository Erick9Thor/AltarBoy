#pragma once

#include "../Utils/UID.h"


enum class ResourceType
{
	UNKNOWN,
	MATERIAL,
	MESH,
	SCENE,
	TEXTURE
};

class Resource
{
public:
	Resource(UID id, ResourceType type);
	virtual ~Resource();

	ResourceType GetType() const;
	UID getID() const;

	virtual void Load();
	virtual void Unload();

private:
	UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
