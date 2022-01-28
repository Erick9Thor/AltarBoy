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

	UID GetID() const { return id; }

	virtual void Load();
	virtual void Unload();

	unsigned int instances = 0;

	std::string file_name = "";
private:
	UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
