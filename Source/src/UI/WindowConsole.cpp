#include "WindowConsole.h"

#include "../Application.h"

#include "../Modules/ModuleEditor.h"
#include <IconsFontAwesome5.h>

#include "../Utils/Logger.h"

#include "imgui.h"


WindowConsole::WindowConsole()
	: Window("Console", true)
{}

WindowConsole::~WindowConsole()
{
}

void WindowConsole::Update()
{
	ImGui::SetNextWindowDockID(App->editor->dock_down_id, ImGuiCond_FirstUseEver);
	if (ImGui::Begin((std::string(ICON_FA_TERMINAL " ") + name).c_str(), &active))
	{
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &autoscroll);
			ImGui::EndPopup();
		}

		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");

		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf_start = buff.begin();
		const char* buf_end = buff.end();

		if (filter.IsActive())
		{
			for (int line_no = 0; line_no < line_offsets.Size; line_no++)
			{
				const char* line_start = buf_start + line_offsets[line_no];
				const char* line_end = (line_no + 1 < line_offsets.Size) ? (buf_start + line_offsets[line_no + 1] - 1) : buf_end;
				if (filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(line_offsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf_start + line_offsets[line_no];
					const char* line_end = (line_no + 1 < line_offsets.Size) ? (buf_start + line_offsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
	}
	ImGui::End();
}

void WindowConsole::Clear()
{
	buff.clear();
	line_offsets.clear();
	line_offsets.push_back(0);
}

void WindowConsole::CleanUp()
{
	Clear();
	filter.Clear();
}
