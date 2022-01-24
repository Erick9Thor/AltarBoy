#include "ModuleFileSystem.h"
#include "../Globals.h"

#include <SDL.h>
#include "physfs.h"
#include "../Utils/Logger.h"

ModuleFileSystem::ModuleFileSystem()
{
	// Create base path to be ready for other modules
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	PHYSFS_mount(".", nullptr, 0);

	PHYSFS_setWriteDir(".");

	// Make sure standard paths exist
	const char* dirs[] = {
		SETTINGS_FOLDER,
		ASSETS_FOLDER,
		LIBRARY_FOLDER,
		LIBRARY_MESH_FOLDER,
		LIBRARY_MATERIAL_FOLDER,
		LIBRARY_SCENE_FOLDER,
		LIBRARY_SHADERS_FOLDER,
		LIBRARY_TEXTURES_FOLDER,
	};

	for (unsigned int i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}
}

ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

bool ModuleFileSystem::Init()
{
	char* write_path = SDL_GetPrefPath("AltarCO", "AltarBoy");
	SDL_free(write_path);

	return true;
}

char* ModuleFileSystem::Load(const char* file_path) const
{
	char* buffer = nullptr;

	PHYSFS_file* fs_file = PHYSFS_openRead(file_path);

	if (!fs_file)
	{
		LOG("Error opening file %s (%s).\n", fs_file, PHYSFS_getLastError());
		return buffer;
	}
	defer
	{
		PHYSFS_close(fs_file);
	};

	PHYSFS_sint32 size = (PHYSFS_sint32) PHYSFS_fileLength(fs_file);

	if (size < 0)
	{
		LOG("File size couldn't be determined for %s (%s).\n", fs_file, PHYSFS_getLastError());
		return buffer;
	}

	buffer = new char[size];
	PHYSFS_sint64 readed = PHYSFS_read(fs_file, buffer, 1, size);

	if (readed < size)
	{
		LOG("Error reading file %s (%s).\n", fs_file, PHYSFS_getLastError());
		return buffer;
	}

	return buffer;
}

bool ModuleFileSystem::Save(const char* file_path, const void* buffer, unsigned int size, bool append) const
{
	bool overwrite = PHYSFS_exists(file_path) != 0;
	PHYSFS_File* file = append ? PHYSFS_openAppend(file_path) : PHYSFS_openWrite(file_path);

	if (!file)
	{
		LOG("Error saving file %s (%s).\n", file_path, strerror(errno));
		return false;
	}
	defer
	{
		PHYSFS_close(file);
	};

	PHYSFS_sint64 written = PHYSFS_write(file, (const void*) buffer, 1, size);
	if (written != size)
	{
		LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		return false;
	}
	else
	{
		if (append == true) {
			LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
		}
		else if (overwrite == false)
		{
			LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
		}
		return true;
	}

	return false;
}

bool ModuleFileSystem::Exists(const char* file_path) const
{
	return PHYSFS_exists(file_path) != 0;
}

bool ModuleFileSystem::IsDirectory(const char* directory_path) const
{
	return PHYSFS_isDirectory(directory_path) != 0;
}

void ModuleFileSystem::CreateDirectory(const char* directory_path) {
	if (!PHYSFS_mkdir(directory_path)) LOG(PHYSFS_getLastError());
}

void ModuleFileSystem::Copy(const char* source_file_path, const char* destination_file_path)
{
	CopyFile(source_file_path, destination_file_path, false);
}

void ModuleFileSystem::Delete(const char* file_path)
{
	if (!PHYSFS_delete(file_path))
	{
		LOG("Can't erase file %s. (%s)\n", file_path, PHYSFS_getLastError());
	}
}

std::string ModuleFileSystem::NormalizePath(const char* full_path) const
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

std::string ModuleFileSystem::GetFileNameAndExtension(const char* file_path) const
{
	const char* last_slash = strrchr(file_path, '/');
	const char* last_backslash = strrchr(file_path, '\\');
	const char* last_separator = last_slash >= last_backslash ? last_slash : last_backslash;

	if (last_separator == nullptr)
	{
		return file_path;
	}

	const char* name_extension = last_separator + 1;
	return name_extension;
}

PathNode ModuleFileSystem::GetAllFiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) const
{
	PathNode root;
	if (Exists(directory))
	{
		root.path = directory;
		SplitFilePath(directory, nullptr, &root.localPath);
		if (root.localPath == "")
			root.localPath = directory;

		std::vector<std::string> file_list, dir_list;
		DiscoverFiles(directory, file_list, dir_list);

		//Adding all child directories
		for (unsigned int i = 0; i < dir_list.size(); i++)
		{
			std::string str = directory;
			str.append(dir_list[i]);
			root.children.push_back(GetAllFiles(str.c_str(), filter_ext, ignore_ext));
		}
		//Adding all child files
		for (unsigned int i = 0; i < file_list.size(); i++)
		{
			//Filtering extensions
			bool filter = true, discard = false;
			if (filter_ext != nullptr)
			{
				filter = HasExtension(file_list[i].c_str(), *filter_ext);
			}
			if (ignore_ext != nullptr)
			{
				discard = HasExtension(file_list[i].c_str(), *ignore_ext);
			}
			if (filter == true && discard == false)
			{
				std::string str = directory;
				str.append("/").append(file_list[i]);
				root.children.push_back(GetAllFiles(str.c_str(), filter_ext, ignore_ext));
			}
		}
		root.isFile = HasExtension(root.path.c_str());
		root.isLeaf = root.children.empty() == true;
	}
	return root;
}

void ModuleFileSystem::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

void ModuleFileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);
		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

bool ModuleFileSystem::HasExtension(const char* path) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool ModuleFileSystem::HasExtension(const char* path, std::vector<std::string> extensions) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
		return true;
	for (unsigned int i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
			return true;
	}
	return false;
}