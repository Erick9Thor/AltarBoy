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

	std::string file_name = "";
	unsigned int instances = 0;

	const char* library_path = "";

private:
	UID id = 0;
	ResourceType type = ResourceType::UNKNOWN;
};
