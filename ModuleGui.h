#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "ImGuiComponents/AppLog.h"

class AppLog;

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
	void ShowAppLog(bool* p_open);


	AppLog* log = nullptr;

private:

};