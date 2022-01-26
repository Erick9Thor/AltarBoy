#pragma once
#include "Window.h"

#include "../Utils/PathNode.h"

#include <vector>

using namespace std;

class WindowProject : public Window
{
public:
	WindowProject();
	~WindowProject() override;
	void Init() override;
	void Update() override;

private:
	void DetailsGUI();
	void DoFilter();
	void CreateBreadCrumps();

	void ChangeDir(char* folder_name);

	void ShowContentDir(PathNode& node);
	void ShowFilesOnFolder();

	void GetAssets();

private:
	char m_filter[128];
	float m_left_column_width = 120;

	PathNode assets;

};
