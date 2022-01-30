#pragma once

#include "../Resources/ResourceModel.h"

#include <assimp/scene.h>

namespace ModelImporter
{
	ResourceModel* ImportModel(const char* file_path);

	void LoadAssimpNode(const aiScene* scene, const aiNode* node);
};
