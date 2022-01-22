#include "ModuleResourceManager.h"

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
