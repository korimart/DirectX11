#include <sstream>
#include "App.h"
#include "DirectXRelated/Texture/GDIPlusManager.h"
#include "Imgui/ImguiManager.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui.h"

using namespace UniChili;

// for initializing and destroying GDI+ library.
GDIPlusManager gdiManager;
// for initializing and destroying Imgui libray.
ImguiManager imguiManager;

UniChili::App::App(int width, int height, const wchar_t* title)
	: window(width, height, title), nanosuit(*window.graphics(), "Models\\nano.gltf")
{
	drawables.reserve(10);

	auto lightBulb = std::make_unique<LightBulb>(*window.graphics());
	lightBulb->setPosition(0.f, 0.f, -5.f);

	pLightBulb = lightBulb.get();

	drawables.push_back(std::move(lightBulb));
}

int UniChili::App::run()
{
	while (true)
	{
		auto exitCode = Window::processWindowMessage();
		if (exitCode) return *exitCode;

		doOneFrame();
	}
}

void UniChili::App::doOneFrame()
{
	auto elapsed = timer.Mark();

	window.graphics()->clearBackBuffer(.5f, .5f, .5f);
	window.graphics()->setView(camera.getView());

	pLightBulb->onViewChange(*window.graphics());

	for (auto& drawable : drawables)
	{
		drawable->update(speedFactor * elapsed);
		drawable->draw(*window.graphics());
	}

	nanosuit.draw(*window.graphics());

	// after drawing our 3D stuff, draw Imgui because
	// Imgui doesn't know about the depth buffer.
	ImGui_ImplDX11_NewFrame(); // must come before win32
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool showImguiDemo = false;
	// If you call ShowDemoWindow demo window pops up regardless 
	// of whether showImguiDemo is true or false. So check it first.
	if (showImguiDemo)
		ImGui::ShowDemoWindow(&showImguiDemo);

	// my window
	if (ImGui::Begin("Hehe Control Window"))
	{
		// camera stuff
		ImGui::SliderFloat("rho", &camera.rho, 0.5f, 20.f);
		ImGui::SliderAngle("phi", &camera.phi, 0.f, 179.f);
		ImGui::SliderAngle("theta", &camera.theta, -180.f, 180.f);

		// material
		static Material mat;
		if (ImGui::ColorPicker3("Material Color", reinterpret_cast<float*>(&mat)))
		{
			// nothing
		}
	}
	ImGui::End();

	// draw to the Imgui internal buffer.
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// present
	window.graphics()->present();
}
