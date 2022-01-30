#include "ResourceScene.h"

ResourceScene::ResourceScene(UID id)
	: Resource(id, ResourceType::SCENE)
{
	library_path = LIBRARY_SCENE_FOLDER;
}
