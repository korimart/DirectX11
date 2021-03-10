#pragma once

namespace UniChili
{
	class ImguiManager
	{
	public:
		ImguiManager();
		ImguiManager(const ImguiManager&) noexcept = delete;
		ImguiManager& operator=(const ImguiManager&) noexcept = delete;
		~ImguiManager();
	};
}