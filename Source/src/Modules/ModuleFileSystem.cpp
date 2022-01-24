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

PathNode ModuleFileSystem::GetAllFiles(const char* directory, std::vector<std::string>* filter_ext) const
{
	PathNode root;
	if (Exists(directory))
	{
		
	}
	return root;
}