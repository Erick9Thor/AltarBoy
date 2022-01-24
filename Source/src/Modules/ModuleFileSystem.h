#pragma once
#include "Module.h"

#include "../Utils/PathNode.h"

#include <string>
#include <vector>

using namespace std;

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem();
	~ModuleFileSystem();

	bool Init() override;

	// --- CRUD --- //
	void CreateDirectory(const char* directory_path);
	char* Load(const char* file_path) const;
	bool Save(const char* file_path, const void* buffer, unsigned int size, bool append = false) const;
	void Delete(const char* file_path);

	// --- Folder utils -- //
	void Copy(const char* source_file_path, const char* destination_file_path);
	std::string NormalizePath(const char* full_path) const;
	bool Exists(const char* file_path) const;
	bool IsDirectory(const char* directory_path) const;
	std::string GetFileNameAndExtension(const char* file_path) const;

	// --- Path utils --- //
	PathNode GetAllFiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) const;

	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;

	bool HasExtension(const char* path) const;
	bool HasExtension(const char* path, std::vector<std::string> extensions) const;
};
