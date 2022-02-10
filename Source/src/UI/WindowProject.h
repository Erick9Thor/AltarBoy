#pragma once
#include "Window.h"

#include "../Utils/PathNode.h"

class WindowProject : public Window
{
public:
	WindowProject();
	~WindowProject() override;
	
	void Init() override;
	void Update() override;

	void CleanUp() override;

private:
	void DoFilter();
	void CreateBreadCrumps();
	void Thumbnail(PathNode& node, float size, bool selected);

	void ShowDir(PathNode& node);
	void ShowFilesOnFolder();

	void GetAssets();

private:

	int GetThumbnailIndex(int i, int j, int columns) const;

	char m_filter[128];
	float m_left_column_width = 120;

	float m_thumbnail_size = 1.f;

	PathNode all_assets;
};
