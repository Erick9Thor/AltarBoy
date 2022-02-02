#pragma once

#include "../Utils/JsonFormaterValue.h"

class Scene;

namespace SceneImporter
{
	Scene* LoadScene(const char* file_path);
	bool SaveScene(Scene* scene, const char* file_path);
}