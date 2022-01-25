#include "WindowProject.h"

#include "../Application.h"
#include "../Globals.h"
#include "../Modules/ModuleFileSystem.h"
#include "../Modules/ModuleResourceManager.h"

#include "ImGuiUtils.h"
#include <IconsFontAwesome5.h>

WindowProject::WindowProject()
	: Window("Project", true)
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

	ShowDir(all_assets);

	ImGui::SameLine();
	ImGuiUtils::VSplitter("vsplit1", &left_size);
	if (left_size.x >= 120)
	{
		m_left_column_width = left_size.x;
	}
	ImGui::SameLine();

	// ShowFilesOnFolder();

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
	const char* c = all_assets.path.c_str();
	char tmp[260];

	while (*c)
	{
		char* c_out = tmp;
		while (*c && *c != '/')
		{
			*c_out = *c;
			++c_out;
			++c;
		}
		*c_out = '\0';
		if (*c == '/') ++c;
		if (ImGui::Button(tmp))
		{
			//TODO
			// changeDir(new_dir);
		}
		ImGui::SameLine();
		ImGui::TextUnformatted("/");
		ImGui::SameLine();
	}
	ImGui::NewLine();
}

//TODO
void WindowProject::Thumbnail(PathNode& node, float size, bool selected)
{
	ImGui::BeginGroup();
	ImVec2 img_size(size, size);
	// RenderInterface* ri = m_app.getRenderInterface();

	ImGuiUtils::Rect(img_size.x, img_size.y, 0xffffFFFF);
	/* TODO */

	ImVec2 text_size = ImGui::CalcTextSize(node.localPath.c_str());
	ImVec2 pos = ImGui::GetCursorPos();
	pos.x += (size - text_size.x) * 0.5f;
	ImGui::SetCursorPos(pos);
	ImGui::Text("%s", node.localPath.c_str());
	ImGui::EndGroup();
	if (selected)
	{
		ImDrawList* dl = ImGui::GetWindowDrawList();
		const unsigned int color = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
		dl->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), color, 0, 0, 3.f);
	}
}

void WindowProject::ShowDir(PathNode& node)
{
	ImVec2 size(max(120.f, m_left_column_width), 0);
	ImGui::BeginChild("left_col", size);
	ImGui::PushItemWidth(120);

	PathNode current_root;
	bool selected_item = false;
	if ((node.path != ASSETS_FOLDER) && ImGui::Selectable("..", &selected_item))
	{
		current_root = node;
	}

	for (auto& subdir : node.children)
	{
		if (!subdir.isFile)
		{
			if (ImGui::Selectable(subdir.localPath.c_str(), &selected_item))
			{
				current_root = subdir;
			}
		}
	}

	if (selected_item)
	{
		all_assets = current_root;
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();
}

void WindowProject::ChangeDir(const char* folder_name)
{
}

void WindowProject::ShowFilesOnFolder()
{
	ImGui::BeginChild("main_col");

	float w = ImGui::GetContentRegionAvail().x;
	int columns = (int) w / int(96 * 1.f);
	columns = max(columns, 1);
	// int tile_count = assets.childrens.empty() ? assets.childrens.size() : 50;
	// int row_count = (tile_count + columns - 1) / columns;
	int row_count = 50;

	auto callbacks = [this](PathNode& node, int idx) {
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", node.localPath.c_str());
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Text("%s", (const char*) node.path.c_str());
			ImGui::SetDragDropPayload("path", node.path.c_str(), strlen(node.path.c_str()) + 1, ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		else if (ImGui::IsItemHovered())
		{
			ImGui::OpenPopup("item_ctx");
		}
	};

	ImGuiListClipper clipper;
	clipper.Begin(row_count);
	while (clipper.Step())
	{
		for (int j = clipper.DisplayStart; j < clipper.DisplayEnd; ++j)
		{
			for (int i = 0; i < columns; ++i)
			{
				if (i > 0) ImGui::SameLine();
				int idx = j * columns + i;
				if (idx < 0)
				{
					ImGui::NewLine();
					break;
				}
				PathNode& node = all_assets.children[idx];
				// bool selected = m_selected_resources.find([&](Resource* res) { return res->getPath().getHash() == tile.file_path_hash; }) >= 0;
				Thumbnail(node, m_thumbnail_size * 96, false);
				callbacks(node, idx);
			}
		}
	}

	bool open_delete_popup = false;
	// FileSystem& fs = m_app.getEngine().getFileSystem();
	static char tmp[260] = "";
	auto common_popup = [&]() {
		const char* base_path = nullptr;

		if (ImGui::BeginMenu("Create directory"))
		{
			ImGui::InputTextWithHint("##dirname", "New directory name", tmp, sizeof(tmp));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				const char* new_dir = /* curent dir path + new dir path */ nullptr;
				App->file_sys->ModuleFileSystem::CreateDir(new_dir);
				ChangeDir(new_dir);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Select all"))
		{
			// TODO
			/*m_selected_resources.clear();
			m_selected_resources.reserve(m_file_infos.size());
			if (m_filtered_file_infos.empty())
			{
				for (const FileInfo& fi : m_file_infos)
				{
					selectResource(Path(fi.filepath), false, true);
				}
			}
			else
			{
				for (int i : m_filtered_file_infos)
				{
					selectResource(Path(m_file_infos[i].filepath), false, true);
				}
			}*/
		}
	};
	if (ImGui::BeginPopup("item_ctx"))
	{
		// ImGui::Text("%s", m_file_infos[m_context_resource].clamped_filename.data);
		ImGui::Separator();

		if (ImGui::BeginMenu("Rename"))
		{
			ImGui::InputTextWithHint("##New name", "New name", tmp, sizeof(tmp));
			if (ImGui::Button("Rename", ImVec2(100, 0)))
			{
				//TODO
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndMenu();
		}
		open_delete_popup = ImGui::MenuItem("Delete");
		ImGui::Separator();
		common_popup();
		ImGui::EndPopup();
	}
	else if (ImGui::BeginPopupContextWindow("context"))
	{
		common_popup();
		ImGui::EndPopup();
	}

	if (open_delete_popup) ImGui::OpenPopup("Delete file");

	if (ImGui::BeginPopupModal("Delete file", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure? This can not be undone.");
		if (ImGui::Button("Yes, delete", ImVec2(100, 0)))
		{
			//TODO Delete resource no fucking idea how but i guess we need to destroy from library first
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 100 - ImGui::GetStyle().WindowPadding.x);
		if (ImGui::Button("Cancel", ImVec2(100, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
}

void WindowProject::GetAssets()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");
	all_assets = App->file_sys->GetAllFiles(ASSETS_FOLDER, nullptr, &ignore_ext);
}
