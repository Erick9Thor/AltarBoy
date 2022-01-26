#include "WindowTimers.h"

#include "imgui.h"

WindowTimers::WindowTimers()
	: Window("Timers", false)
{
	timer.Start();
}

WindowTimers::~WindowTimers()
{
}

void WindowTimers::Update()
{
	if (ImGui::Begin(name, &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Game Timer");
		ImGui::SameLine();
		ImVec4 color(1.0f, 1.0f, 1.0f, 1.0f);
		const char* state = "Running";
		if (!GameTimer::running)
		{
			color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			state = "Stopped";
		}
		else if (GameTimer::paused)
		{
			color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			state = "Paused";
		}		
		ImGui::TextColored(color, "(%s): %.2f", state, GameTimer::total_time);

		ImGui::Separator();
		ImGui::Text("Test Timer");
		ImGui::SameLine();
		ImGui::Text("%.2f", timer.Read());
	}

	ImGui::End();
}
