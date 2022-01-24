#pragma once
#include "Module.h"

#include "../Utils/PathNode.h"

#include "../Resources/Resource.h"

#include <map>

using namespace std;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager();
	~ModuleResourceManager() override;

	// --- Lifecycle --- //
	bool Init() override;
	bool CleanUp() override;

	// --- Asset importer --- //
	void LoadAllAssetsFolder();
	void LoadByNode(PathNode node);

	void ImportExternalFile(char* file_path);
	UID ImportFileFromAssets(const char* file_path);
	ResourceType GetTypeFromFileExtension(const char* file_path) const;

	// --- Resource DB mange --- //
	Resource* RequestResource(UID ID);
	std::string GenerateResourcePath(UID id) const;

	void SaveResource(Resource* resource);
	void UnloadResource(UID ID);

private:
	Resource* CreateNewResourceByType(const char* assetsFile, ResourceType type);
	std::map<UID, Resource*> resources;
};
