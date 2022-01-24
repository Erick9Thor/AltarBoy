#include "ModuleResourceManager.h"

#include "../Globals.h"

#include "../Application.h"
#include "../Modules/ModuleFileSystem.h"

#include "../Importers/MaterialImporter.h"
#include "../Importers/ModelImporter.h"
#include "../Importers/SceneImporter.h"
#include "../Utils/Logger.h"

#include "../Resources/ResourceMaterial.h"
#include <ilu.h>


ModuleResourceManager::ModuleResourceManager()
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	ilInit();
	iluInit();
	ilEnable(IL_KEEP_DXTC_DATA);
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

	LoadAllAssetsFolder();

	return true;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

void ModuleResourceManager::LoadAllAssetsFolder()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");

	PathNode assets = App->file_sys->GetAllFiles(ASSETS_FOLDER, nullptr, &ignore_ext);
	LoadByNode(assets);
}

void ModuleResourceManager::LoadByNode(PathNode node)
{
	bool importedAsNew = false;

	std::string metaFile = node.path + ".meta";
	if (App->file_sys->Exists(metaFile.c_str()))
	{
		char* buffer = App->file_sys->Load(metaFile.c_str());
	}
	else
	{
		ImportFileFromAssets(node.path.c_str());
	}
}

void ModuleResourceManager::ImportExternalFile(char* file_path)
{
	std::string normalized_droped_path = App->file_sys->NormalizePath(file_path);
	std::string new_file_path = std::string(ASSETS_FOLDER) + App->file_sys->GetFileNameAndExtension(normalized_droped_path.c_str());
	
	App->file_sys->Copy(file_path, new_file_path.c_str()); //TODO: Check if duplicated file


	/*
		1. Create new resource
		2. Set this new resource into the correct folder
	*/

	UID resource_id = ImportFileFromAssets(new_file_path.c_str());

	//Find parent folder, add child resource and save folder content
	/*if (const ResourceBase* folderBase = FindResourceBase(dstDir, nullptr, ResourceType::FOLDER))
	{
		ResourceHandle<Resource> hFolder(folderBase->ID);
		hFolder.Get()->AddContainedResource(newResourceID);
		SaveResource(hFolder.Get());
	}*/
}

UID ModuleResourceManager::ImportFileFromAssets(const char* file_path)
{
	ResourceType type = GetTypeFromFileExtension(file_path);
	if (type == ResourceType::UNKNOWN) return 0;

	Resource* resource = CreateNewResourceByType(file_path, type);
	UID resource_uid = resource->getID();

	char* file_buffer = App->file_sys->Load(file_path);

	SaveResource(resource);
	RELEASE_ARRAY(file_buffer);

	UnloadResource(resource_uid);

	return resource_uid;
}

Resource* ModuleResourceManager::CreateNewResourceByType(const char* file_path, ResourceType type)
{
	std::string resourceFilePath = GenerateResourcePath(GenerateUID());
	Resource* resource = nullptr;
	/*switch (type)
	{
		case ResourceType::MATERIAL:
			resource = new ResourceMaterial(resourceName, assetFilePath, resourceFilePath.c_str());
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
	}*/
	return resource;
}


ResourceType ModuleResourceManager::GetTypeFromFileExtension(const char* file_path) const
{
	return ResourceType();
}

Resource* ModuleResourceManager::RequestResource(UID ID)
{
	return nullptr;
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
	std::map<UID, Resource*>::iterator it = resources.find(ID);
	if (it != resources.end())
	{
		it->second->Unload();
		RELEASE(it->second);
		resources.erase(it);
	}
}

