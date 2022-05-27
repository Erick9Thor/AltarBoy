#include "core/hepch.h"

#include "ResourceScene.h"

Hachiko::ResourceScene::ResourceScene() : Resource(Resource::Type::SCENE) {}

Hachiko::ResourceScene::ResourceScene(UID id) : Resource(id, Resource::Type::SCENE) {}
