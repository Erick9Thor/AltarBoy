#include "ResourceHandle.h"

#include "../Application.h"

#include "../Modules/ModuleResourceManager.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceScene.h"
#include "ResourceTexture.h"

template class ResourceHandle<Resource>;
template class ResourceHandle<ResourceMesh>;
template class ResourceHandle<ResourceMaterial>;
template class ResourceHandle<ResourceScene>;
template class ResourceHandle<ResourceTexture>;

template <typename T>
T* ResourceHandle<T>::RequestResource() const
{
	return dynamic_cast<T*>(App->resource_manager->RequestResource(ID));
}

template <typename T>
void ResourceHandle<T>::Free()
{
	if (resource != nullptr && App != nullptr)
	{
		// App->resource_manager->ReleaseResource(resource);
		resource = nullptr;
	}
}


