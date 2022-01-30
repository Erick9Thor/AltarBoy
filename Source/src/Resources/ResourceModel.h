#pragma once
#include "Resource.h"

#include "../Globals.h"

class ResourceModel : public Resource
{
public:
	ResourceModel(UID uid);
	~ResourceModel() override;
};
