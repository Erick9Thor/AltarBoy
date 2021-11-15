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
	ModuleGui() :fps_log(100), ms_log(100) {};
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

	update_status showBasicMenu();
	
	void showFPSGraph();
	void showConfigWindow();
	void showHardwareInfo();
	void showAbaoutInfo();

private:
	std::vector<float> ms_log;
	std::vector<float> fps_log;
};