#include "ModuleLoadModels.h"
#include "../Application.h"
#include "../ModuleCamera.h"
#include "../ModuleTexture.h"
#include "../ModuleWindow.h"
#include "../Model.h"

bool ModuleLoadModels::Init()
{
	LOG("Model: Creating Model");
	ourModel = new Model("BakerHouse.fbx");

	return true;
}

update_status ModuleLoadModels::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleLoadModels::Update()
{
	ourModel->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleLoadModels::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleLoadModels::CleanUp()
{
	return false;
}

void ModuleLoadModels::LoadModel(const char* _fileName)
{

	ourModel->CleanUp();
	ourModel = new Model(_fileName);
}
