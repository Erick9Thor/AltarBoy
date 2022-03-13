#include "Core/hepch.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"

Hachiko::ResourceModel::ResourceModel(UID uid)
	: Resource(uid, Type::MODEL)
{
}