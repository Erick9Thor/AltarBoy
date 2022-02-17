#include "core/hepch.h"

Hachiko::Resource::Resource(const UID id, const Type type) :
    id(id),
    type(type) {}

Hachiko::Resource::~Resource() = default;
