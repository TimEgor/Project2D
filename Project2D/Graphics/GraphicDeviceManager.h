#pragma once

#include <Graphics/GraphicDevice.h>

class GraphicDeviceManager final {
private:
	GraphicDevice* currentGraphicDevice = nullptr;

	GraphicDeviceManager() = default;

public:
	~GraphicDeviceManager() { releaseCurrentDevice(); }

	static GraphicDeviceManager& get();

	GraphicDevice* initNewDevice(GraphicalDeviceType type);
	void releaseCurrentDevice();

	GraphicDevice* getCurrentDevice();
};