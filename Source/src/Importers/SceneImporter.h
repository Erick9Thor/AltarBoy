#pragma once

#include "../Resources/ResourceScene.h"

#include "../Utils/JsonFormaterValue.h"

class Scene;

namespace SceneImporter
{
	ResourceScene* ImportScene(const char* file_path);
	Scene* LoadScene(const char* file_path);
	bool SaveScene(Scene* scene, const char* file_path);
}