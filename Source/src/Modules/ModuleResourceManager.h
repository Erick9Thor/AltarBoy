#pragma once
#include "Module.h"

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager();
	~ModuleResourceManager() override;

	// --- Lifecycle --- //
	bool Init() override;
	update_status Update(const float delta) override;
	bool CleanUp() override;


	void ImportExternalFile(char* file_name);
};
