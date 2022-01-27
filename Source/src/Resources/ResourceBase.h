#pragma once

#include <string>
#include <vector>

#include "../Globals.h"
#include "../Utils/JsonFormaterValue.h"
#include "Resource.h"

struct ResourceBase
{
	ResourceBase() {};
	ResourceBase(ResourceType type, const char* file, const char* name, UID id) : type(type), assetsFile(file), name(name ? name : ""), ID(id) {};

	bool Compare(const char* file, const char* name, ResourceType type) const
	{
		return (this->assetsFile == file && (name ? this->name == name : true) && (type != ResourceType::UNKNOWN ? type == this->type : true));
	}

	void Serialize(JsonFormaterValue j_meta, unsigned& resource_index)
	{
		j_meta["ID"] = ID;
		j_meta["Name"] = name.c_str();
		j_meta["Library_file"] = libraryFile.c_str();

		int intType = static_cast<int>(type);
		j_meta["Type"] = intType;
		type = static_cast<ResourceType>(intType);
	}

	UID ID = 0;
	ResourceType type = ResourceType::UNKNOWN;

	std::string name = "";
	std::string assetsFile = "";
	std::string libraryFile = "";

	std::vector<unsigned int> containedResources;
};
