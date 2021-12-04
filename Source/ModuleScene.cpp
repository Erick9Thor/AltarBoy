#include "ModuleScene.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleWindow.h"
#include "Model.h"

bool ModuleScene::Init()
{
	LOG("Model: Creating Model");
	scene_model = new Model("BakerHouse.fbx");

	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	scene_model->Draw();
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
	delete scene_model;
	scene_model = new Model(_fileName);
	App->camera->FocusOnModel(scene_model);
}

void ModuleScene::DrawGui()
{
	scene_model->DrawGui();
}
