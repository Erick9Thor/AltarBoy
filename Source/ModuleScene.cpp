#include "ModuleScene.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleWindow.h"
#include "Model.h"

bool ModuleScene::Init()
{
	LOG("Model: Creating Model");
	ourModel = new Model("BakerHouse.fbx");

	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	ourModel->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return false;
}

void ModuleScene::LoadModel(const char* _fileName)
{
	delete ourModel;

	// TODO: Path file formating
	ourModel = new Model(_fileName);
}
