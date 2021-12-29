#pragma once

#include "Globals.h"

class Application;

class Module
{
public:
	Module()
	{
	}

	virtual ~Module() {};

	virtual bool Init()
	{
		return true;
	}

	virtual update_status PreUpdate(const float delta)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(const float delta)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(const float delta)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void DrawDebug()
	{}

	virtual void DrawGui() {}
};
