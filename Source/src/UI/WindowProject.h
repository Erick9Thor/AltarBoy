#pragma once
#include "Window.h"

#include <vector>

using namespace std;

class WindowProject : public Window
{
public:
	WindowProject();
	void Update() override;

private:
	void DetailsGUI();
	void DoFilter();
	void CreateBreadCrumps();
	void ShowContentDir();
	void ShowFilesOnFolder();

	void GetAssets();

private:
	char m_filter[128];
	float m_left_column_width = 120;

	std::vector<std::string> assets;

};
