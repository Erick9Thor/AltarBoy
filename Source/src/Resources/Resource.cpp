#include "Resource.h"

Resource::Resource(Hachiko::UID id, ResourceType type)
	: id(id)
	, type(type)
{
}

Resource::~Resource()
{
}
