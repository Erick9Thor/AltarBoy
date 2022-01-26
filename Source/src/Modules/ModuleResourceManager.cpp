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
	std::string metaFile = node.path + META_EXTENSION;
	if (App->file_sys->Exists(metaFile.c_str()))
	{
		rapidjson::Document document;
		JsonFormaterValue jMeta(document, document);

		const char* buffer = App->file_sys->Load(metaFile.c_str());
		ReadJSON(buffer, document);
	
		ImportAssetByExtension(jMeta, node.path.c_str());
	}
	else
	{
		ImportFileFromAssets(node.path.c_str());
	}

	if (!node.isFile && !node.isLeaf)
	{
		/*ResourceHandle<Resource> hFolder(assetID);
		std::vector<uint64> newChildren;
		for (uint i = 0; i < node.children.size(); i++)
		{
			uint64 childID = 0;
			LoadAssetBase(node.children[i], childID);
			if (!hFolder.Get()->HasContainedResource(childID))
				newChildren.push_back(childID);
		}
		if (newChildren.size() > 0) //New resources from this folder have been imported. Update folder content
		{
			for (uint i = 0; i < newChildren.size(); ++i)
				hFolder.Get()->AddContainedResource(newChildren[i]);
			SaveResource(hFolder.Get());
		}*/
	}
}

void ModuleResourceManager::ImportAssetByExtension(JsonFormaterValue jMeta, const char* file_path)
{
	std::string extension = App->file_sys->GetFileExtension(file_path);

	const char* buffer = App->file_sys->Load(file_path);
	if (sizeof(buffer) == 0)
	{
		LOG("Error loading scene %s", file_path);
		return;
	}

	Resource* resource = nullptr;
	if (extension == SCENE_EXTENSION)
	{
		SceneImporter::ImportScene(buffer, jMeta);
	}
	/*else if (extension == MATERIAL_EXTENSION)
	{
		// Material files
		// MaterialImporter::ImportMaterial(filePath, jMeta);
	}
	else if (extension == FRAGMENT_SHADER_EXTENSION || extension == VERTEX_SHADER_EXTENSION || extension == DEFAULT_SHADER_EXTENSION)
	{
		// Shader files
		// ShaderImporter::ImportShader(filePath, jMeta);
	}
	else if (extension == MODEL_EXTENSION)
	{
		// Model files
		// ModelImporter::ImportModel(filePath, jMeta);
	}
	else if (extension == SKYBOX_EXTENSION)
	{
		// Skybox files
		// SkyboxImporter::ImportSkybox(filePath, jMeta);
	}
	else if (extension == JPG_TEXTURE_EXTENSION || extension == PNG_TEXTURE_EXTENSION)
	{
		// Texture files
		// TextureImporter::ImportTexture(filePath, jMeta);
	}
	else if (extension == PREFAB_EXTENSION)*/

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

void ModuleResourceManager::ReadJSON(const char* buffer, rapidjson::Document& document)
{
	// Parse document from file
	document.Parse<rapidjson::kParseNanAndInfFlag>(buffer);
	if (document.HasParseError())
	{
		LOG("Error parsing JSON");
	}
}