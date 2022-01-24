#include "WindowProject.h"

#include "../Application.h"
#include "../Globals.h"
#include "../Modules/ModuleFileSystem.h"

#include "ImGuiUtils.h"
#include <IconsFontAwesome5.h>

WindowProject::WindowProject()
	: Window("Project", true) {

}

void WindowProject::Update()
{
	GetAssets();
	/*if (!ImGui::Begin(ICON_FA_IMAGES "Assets##assets", &active))
	{
		ImGui::End();
		DetailsGUI();
		return;
	}

	/*ImGui::PushItemWidth(100);
	if (ImGui::InputTextWithHint("##filter", "Filter", m_filter, sizeof(m_filter))) DoFilter();
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGuiUtils::IconButton(ICON_FA_TIMES, "Reset filter"))
	{
		m_filter[0] = '\0';
		DoFilter();
	}

	ImGui::SameLine();
	// CreateBreadCrumps();
	ImGui::Separator();

	/*float content_w = ImGui::GetContentRegionAvail().x;
	ImVec2 left_size(m_left_column_width, 0);
	if (left_size.x < 10) left_size.x = 10;
	if (left_size.x > content_w - 10) left_size.x = content_w - 10;

	ShowContentDir();

	ImGui::SameLine();
	ImGuiUtils::VSplitter("vsplit1", &left_size);
	if (left_size.x >= 120)
	{
		m_left_column_width = left_size.x;
	}
	ImGui::SameLine();

	ShowFilesOnFolder();

	ImGui::End();*/
}

void WindowProject::DetailsGUI()
{
	if (ImGui::Begin(ICON_FA_IMAGE "Asset inspector##asset_inspector", &active, ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		if (true) //TODO: Case size of folder is > 1
		{
			
		}

		if (false) // CAse resurce is empty
		{
			ImGui::End();
			return;
		}
		if (false) // Case resource size is 1
		{
		}
	}
}

//TODO
void WindowProject::DoFilter() 
{

}

//TODO
void WindowProject::CreateBreadCrumps()
{
}

void WindowProject::ShowContentDir()
{
	ImVec2 size(max(120.f, m_left_column_width), 0);
	ImGui::BeginChild("left_col", size);
	ImGui::PushItemWidth(120);
	bool b = false;
	/*if ((m_dir[0] != '.' || m_dir[1] != 0) && ImGui::Selectable("..", &b))
	{
		char dir[120];
		copyString(Span(dir), Path::getDir(m_dir));
		changeDir(dir);
	}

	for (auto& subdir : m_subdirs)
	{
		if (ImGui::Selectable(subdir, &b))
		{
			StaticString<LUMIX_MAX_PATH> new_dir(m_dir, "/", subdir);
			changeDir(new_dir);
		}
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();*/
}

//TODO
void WindowProject::ShowFilesOnFolder()
{
}

void WindowProject::GetAssets()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");
	// assets = App->file_sys->GetAllFilesInFolder(ASSETS_FOLDER);
	// assets = App->file_sys->GetAllFilesInFolder("Assets", filter_value, ignored_files);
}

