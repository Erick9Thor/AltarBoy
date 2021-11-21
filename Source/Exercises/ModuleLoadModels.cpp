#include "ModuleLoadModels.h"
#include "../Program.h"
#include "../Application.h"
#include "../ModuleCamera.h"
#include "../ModuleTexture.h"
#include "../ModuleWindow.h"
#include "../Model.h"

bool ModuleLoadModels::Init()
{
	LOG("Shaders: Creating program");
	App->program->CreateProgramID("Bakerhouse\\model_loading.vs", "Bakerhouse\\model_loading.fs");
	LOG("Shaders: Shaders created correctly");


	LOG("Model: Creating Model");
	ourModel = new Model("Bakerhouse\\BakerHouse.fbx");

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
