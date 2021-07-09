#include "GraphicDeviceManager.h"

#include <Graphics/D3D11/D3D11Device.h>

GraphicDeviceManager& GraphicDeviceManager::get() {
	static GraphicDeviceManager uniqueGraphicDeviceManager;
	return uniqueGraphicDeviceManager;
}

GraphicDevice* GraphicDeviceManager::initNewDevice(GraphicalDeviceType type) {
	GraphicDevice* newDevice = nullptr;
	if (!currentGraphicDevice) {
		switch (type) {
#ifdef RENDERING_D3D11
		case D3D11_GraphicalDeviceType:
			newDevice = new D3D11Device();
			break;
#endif // D3D11_RENDERING

		default:
			break;
		}

		if (newDevice) {
			currentGraphicDevice = newDevice;
			currentGraphicDevice->init();
		}
	}

	return newDevice;
}

void GraphicDeviceManager::releaseCurrentDevice() {
	if (currentGraphicDevice) {
		currentGraphicDevice->release();
		delete currentGraphicDevice;
		currentGraphicDevice = nullptr;
	}
}

GraphicDevice* GraphicDeviceManager::getCurrentDevice() {
	return currentGraphicDevice;
}
