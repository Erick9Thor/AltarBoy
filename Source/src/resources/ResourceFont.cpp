#include "core/hepch.h"

#include "ResourceFont.h"

Hachiko::ResourceFont::ResourceFont() : Resource(Resource::Type::FONT) {}

Hachiko::ResourceFont::ResourceFont(UID id) : Resource(id, Resource::Type::FONT) {}
