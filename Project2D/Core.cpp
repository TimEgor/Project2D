#include "Core.h"

#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <LevelManager/LevelManager.h>
#include <Graphics/GraphicDeviceManager.h>

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

	return true;
}

void Core::release() {
	ResourceManager::get().release();
	UserInterfaces::get().release();

	GraphicDeviceManager::get().releaseCurrentDevice();
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

	GraphicDeviceManager::get().initNewDevice(D3D11_GraphicalDeviceType);

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
			//RenderingData& renderingData = level->getRenderingData();
			//renderingData.getSceneRedneringOrderManager().sortForwardNodes();
			//renderingData.getCanvasRedneringOrderManager().sortForwardNodes();

			//d3d11Renderer.beginDrawing();
			//d3d11Renderer.draw(renderingData);
			//d3d11Renderer.endDrawing();
		}

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}
}
