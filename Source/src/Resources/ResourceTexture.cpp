#include "ResourceTexture.h"


ResourceTexture::ResourceTexture(UID id)
	: Resource(id, ResourceType::TEXTURE)
{
	library_path = LIBRARY_TEXTURES_FOLDER;
}
