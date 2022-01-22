#include "ModuleResourceManager.h"

#include "../Application.h"
#include "../Modules/ModuleFileSystem.h"

#include "../Importers/MaterialImporter.h"
#include "../Importers/ModelImporter.h"
#include "../Importers/SceneImporter.h"
#include "../Utils/Logger.h"

ModuleResourceManager::ModuleResourceManager()
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	return true;
}

update_status ModuleResourceManager::Update(const float delta)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

void ModuleResourceManager::ImportExternalFile(char* file_name)
{
}

UID ModuleResourceManager::ImportFileFromAssets(const char* file_path)
{
	ResourceType type = GetTypeFromFileExtension(file_path);
	if (type == ResourceType::UNKNOWN) return 0;

	Resource* resource = CreateNewResource(file_path, type);
	UID resource_uid = resource->getID();

	char* file_buffer = App->file_sys->Load(file_path);

	SaveResource(resource);
	RELEASE_ARRAY(file_buffer);

	UnloadResource(resource_uid);

	return resource_uid;
}

Resource* ModuleResourceManager::CreateNewResource(const char* file_path, ResourceType type)
{
	std::string resourceFilePath = GenerateResourcePath(GenerateUID());
	Resource* resource = nullptr;
	/*switch (type)
	{
	case ResourceType::MATERIAL:
		resource = new ResourceMaterial(id, resourceName, assetFilePath, resourceFilePath.c_str());
		break;
	case ResourceType::MESH:
		resource = new ResourceMesh(id, resourceName, assetFilePath, resourceFilePath.c_str());
		break;
	case ResourceType::PREFAB:
		resource = new ResourcePrefab(id, resourceName, assetFilePath, resourceFilePath.c_str());
		break;
	case ResourceType::SCENE:
		resource = new ResourceScene(id, resourceName, assetFilePath, resourceFilePath.c_str());
		break;
	case ResourceType::TEXTURE:
		resource = new ResourceTexture(id, resourceName, assetFilePath, resourceFilePath.c_str());
		break;
	default:
		LOG("Resource of type %i hasn't been registered in ModuleResources::CreateResourceByType.", (unsigned) type);
		assert(false); // ERROR: Resource type not registered
		return nullptr;
	}
	*/
	return resource;
}


ResourceType ModuleResourceManager::GetTypeFromFileExtension(const char* file_path) const
{
	return ResourceType();
}

std::string ModuleResourceManager::GenerateResourcePath(UID id) const
{
	std::string strId = std::to_string(id);
	std::string metaFolder = std::string(LIBRARY_FOLDER) + strId.substr(0, 2);

	if (!App->file_sys->Exists(metaFolder.c_str()))
	{
		// TODO: App->file_sys->CreateFolder(metaFolder.c_str()); Create a new folder in case not exist
	}

	return metaFolder + "/" + strId;
}

void ModuleResourceManager::SaveResource(Resource * resource)
{
}

void ModuleResourceManager::UnloadResource(UID ID)
{
}

