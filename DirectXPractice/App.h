#pragma once
#include "WindowRelated/Window.h"
#include "Helpers/ChiliTimer.h"
#include "DirectXRelated/Drawables/Box.h"
#include "DirectXRelated/Texture/Surface.h"
#include "DirectXRelated/Drawables/TexturePlane.h"
#include "Images/ImagePaths.h"
#include "DirectXRelated/RotationalCamera.h"
#include "DirectXRelated/Drawables/LightBulb.h"
#include "DirectXRelated/Drawables/Material.h"

namespace UniChili
{
	class App
	{
	public:
		App(int width, int height, const wchar_t* title);

		App(const App&) = delete;
		App& operator=(const App&) = delete;

	public:
#pragma region interface

		int run();

#pragma endregion
	private:
		void doOneFrame();

	private:
		RotationalCamera camera;
		Window window;
		ChiliTimer timer;
		std::vector<std::unique_ptr<Drawable>> drawables;
		Box* pBox = nullptr;
		float speedFactor = 1.0f;
		LightBulb* pLightBulb = nullptr;
	};
}