#pragma once
#include "Module.h"

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

	// --- Utility functions -- //
	bool Copy(const char* source_file_path, const char* destination_file_path);

	bool Exists(const char* file_path) const;
	bool IsDirectory(const char* directory_path) const;
};
