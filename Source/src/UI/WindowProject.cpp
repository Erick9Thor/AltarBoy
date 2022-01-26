#include "WindowProject.h"

#include "../Application.h"
#include "../Globals.h"
#include "../Modules/ModuleFileSystem.h"
#include "../Modules/ModuleResourceManager.h"

#include "ImGuiUtils.h"
#include <IconsFontAwesome5.h>

WindowProject::WindowProject()
	: Window("Project", true) {

}

WindowProject::~WindowProject()
{
}

void WindowProject::Init()
{
	GetAssets();
}

void WindowProject::Update()
{
	if (!ImGui::Begin(ICON_FA_IMAGES "Assets##assets", &active))
	{
		ImGui::End();
		DetailsGUI();
		return;
	}

	ImGui::PushItemWidth(100);
	if (ImGui::InputTextWithHint("##filter", "Filter", m_filter, sizeof(m_filter))) DoFilter();
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGuiUtils::IconButton(ICON_FA_TIMES, "Reset filter"))
	{
		m_filter[0] = '\0';
		DoFilter();
	}

	ImGui::SameLine();
	CreateBreadCrumps();
	ImGui::Separator();

	float content_w = ImGui::GetContentRegionAvail().x;
	ImVec2 left_size(m_left_column_width, 0);
	if (left_size.x < 10) left_size.x = 10;
	if (left_size.x > content_w - 10) left_size.x = content_w - 10;

	ShowContentDir(assets);

	ImGui::SameLine();
	ImGuiUtils::VSplitter("vsplit1", &left_size);
	if (left_size.x >= 120)
	{
		m_left_column_width = left_size.x;
	}
	ImGui::SameLine();

	ShowFilesOnFolder();

	ImGui::End();
}

void WindowProject::DetailsGUI()
{
	if (ImGui::Begin(ICON_FA_IMAGE "Asset inspector##asset_inspector", &active, ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		ImGui::End();
	}
}

//TODO
void WindowProject::DoFilter() 
{

}

void WindowProject::CreateBreadCrumps()
{
	
}



void WindowProject::ShowContentDir(PathNode& node)
{
	ImVec2 size(max(120.f, m_left_column_width), 0);
	ImGui::BeginChild("left_col", size);
	ImGui::PushItemWidth(120);

	bool b = false;
	if ((node.path != ASSETS_FOLDER) && ImGui::Selectable("..", &b))
	{
		// Set new dir
		
	}

	for (auto& subdir : node.children)
	{
		if (ImGui::Selectable(subdir.localPath.c_str(), &b))
		{
			// Chnge directory
			assets = App->file_sys->GetAllFiles(node.localPath.c_str(), nullptr, nullptr);
		}
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();
}

void WindowProject::ChangeDir(char* folder_name)
{
}

void WindowProject::ShowFilesOnFolder()
{
	ImGui::BeginChild("ExplorerFolder", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_NoScrollbar);

	// ImGui::Text(Engine->moduleEditor->GetCurrentExplorerDirectory());
	ImGui::Separator();

	ImGui::BeginChild("ImagesChild");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(50, 30));

	ImVec2 windowCursorPosition = ImGui::GetCursorPos();
	unsigned int itemIndex = 0;

	// print folders and shits

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::EndChild();
}

void WindowProject::GetAssets()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");
	assets = App->file_sys->GetAllFiles(ASSETS_FOLDER, nullptr, &ignore_ext);
}

