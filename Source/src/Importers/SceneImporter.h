#pragma once

#include "../Utils/JsonFormaterValue.h"

class Scene;

namespace SceneImporter
{
	bool ImportScene(const char* file_path, JsonFormaterValue j_meta);

	Scene* LoadScene(const char* file_path);
	bool SaveScene(Scene* scene, const char* file_path);
}