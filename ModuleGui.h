#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"

#include "ImGuiComponents/AppLog.h"


class ModuleGui : public Module
{
public:
	ModuleGui();
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void ShowAboutWindow(bool* p_open);

private:

};