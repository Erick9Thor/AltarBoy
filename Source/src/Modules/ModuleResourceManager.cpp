#include "ModuleResourceManager.h"

#include "../Globals.h"

#include "../Application.h"
#include "../Modules/ModuleFileSystem.h"

#include "../Utils/Logger.h"

#include "../Importers/MaterialImporter.h"
#include "../Importers/ModelImporter.h"
#include "../Importers/SceneImporter.h"

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
	std::string metaFile = node.path + META_EXTENSION;
	/*if (App->file_sys->Exists(metaFile.c_str()))
	{
		//TODO
	}
	else
	{
		ImportFileFromAssets(node.path.c_str());
	}

	if (!node.isFile && !node.isLeaf) // Case we are loading a folder
	{
		//TODO
	}*/
}

Resource* ModuleResourceManager::ImportAssetByExtension(const char* file_path)
{
	std::string extension = App->file_sys->GetFileExtension(file_path);

	const char* buffer = App->file_sys->Load(file_path);
	if (sizeof(buffer) == 0)
	{
		LOG("Error loading scene %s", file_path);
		return nullptr;
	}

	Resource* resource = nullptr;
	if (extension == SCENE_EXTENSION) //JSON file
	{
		resource = SceneImporter::ImportScene(buffer);
	}
	else if (extension == MATERIAL_EXTENSION) // Binary file
	{
		// resource = MaterialImporter::Material::Import(???);
	}
	else if (extension == MODEL_EXTENSION)
	{
		resource = ModelImporter::ImportModel(file_path);
	}
	else if (extension == JPG_TEXTURE_EXTENSION || extension == PNG_TEXTURE_EXTENSION)
	{
		// resource = MaterialImporter::Textures::Import(buffer);
	}
	else
	{
		return nullptr;
	}

	return resource;
}


void ModuleResourceManager::ImportExternalFile(char* file_path)
{
	std::string normalized_droped_path = App->file_sys->NormalizePath(file_path);
	std::string new_file_path = std::string(ASSETS_FOLDER) + App->file_sys->GetFileNameAndExtension(normalized_droped_path.c_str());

	if (App->file_sys->Exists(new_file_path.c_str()))
	{
		//TODO: Get the resource existing from the importet and check if is changed if is changed reimport data or just reimport all :D
	}
	else
	{
		App->file_sys->Copy(file_path, new_file_path.c_str()); 
	}

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
	Resource* resource = ImportAssetByExtension(file_path);

	if (!resource) {
		return 0;
	}

	UID resource_uid = resource->GetID();

	SaveResource(resource);
	// RELEASE_ARRAY(file_buffer);

	UnloadResource(resource_uid);

	return resource_uid;
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
{}

void ModuleResourceManager::ReadJSON(const char* buffer, rapidjson::Document& document)
{
	// Parse document from file
	document.Parse<rapidjson::kParseNanAndInfFlag>(buffer);
	if (document.HasParseError())
	{
		LOG("Error parsing JSON");
	}
}