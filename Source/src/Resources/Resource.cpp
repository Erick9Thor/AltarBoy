#include "Resource.h"

Hachiko::Resource::Resource(UID id, Type type) :
    id(id),
    type(type) {}

Hachiko::Resource::~Resource() = default;
