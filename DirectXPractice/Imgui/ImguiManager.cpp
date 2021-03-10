#include "ImguiManager.h"
#include "imgui.h"

UniChili::ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

UniChili::ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
