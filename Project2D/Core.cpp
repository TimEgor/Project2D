#include "Core.h"

#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <LevelManager/LevelManager.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11Renderer.h>
#include <Graphics/FontManager.h>
#include <Graphics/RenderingData.h>

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millisecond;

#define CHECK_INITIALIZATION(check_val) { if (!check_val) { release(); return false; }}

Core& Core::get() {
	static Core uniqueCore;
	return uniqueCore;
}

bool Core::init() {
	CHECK_INITIALIZATION(ResourceManager::get().init())
	CHECK_INITIALIZATION(UserInterfaces::get().init());
	CHECK_INITIALIZATION(D3D11::get().init());
	CHECK_INITIALIZATION(D3D11Renderer::get().init());
	CHECK_INITIALIZATION(FontManager::get().init());

	return true;
}

void Core::release() {
	ResourceManager::get().release();
	FontManager::get().release();
	D3D11Renderer::get().release();
	D3D11::get().release();
	UserInterfaces::get().release();
}

void Core::run() {
	Clock::time_point startTime = Clock::now();
	Clock::time_point currentTime, lastFrameStartTime;
	lastFrameStartTime = startTime;

	std::chrono::duration<float> deltaClockTime;
	float deltaTime;

	UserInterfaces& userInterfaces = UserInterfaces::get();
	Input* input = UserInterfaces::get().getInput();	
	LevelManager& levelManager = LevelManager::get();
	D3D11Renderer& d3d11Renderer = D3D11Renderer::get();

	MSG msg{ 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		currentTime = Clock::now();
		deltaClockTime = currentTime - lastFrameStartTime;
		deltaTime = deltaClockTime.count();

		input->update();

		Level* level = levelManager.getCurrentLevel();
		if (level) {
			//Logic
			level->update(deltaTime);

			//Rendering
			RenderingData& renderingData = level->getRenderingData();
			renderingData.getSceneRedneringOrderManager().sortNodes();
			renderingData.getCanvasRedneringOrderManager().sortNodes();

			d3d11Renderer.beginDrawing();
			d3d11Renderer.draw(renderingData);
			d3d11Renderer.endDrawing();
		}

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}
}
