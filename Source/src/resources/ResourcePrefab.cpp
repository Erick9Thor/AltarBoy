#include "core/hepch.h"

#include "ResourcePrefab.h"

Hachiko::ResourcePrefab::ResourcePrefab() : Resource(Resource::Type::PREFAB) {}

Hachiko::ResourcePrefab::ResourcePrefab(UID id) : Resource(id, Resource::Type::PREFAB) {}
