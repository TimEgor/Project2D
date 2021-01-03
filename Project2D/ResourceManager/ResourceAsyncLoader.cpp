#include "ResourceAsyncLoader.h"

#include <ResourceManager/ResourceManager.h>
#include <ResourceManager/ResourceCreator.h>
#include <ResourceManager/Zip/ZipFileSystem.h>

void ResourceAsyncLoader::loadingFunction(void* args) {
    ResourceAsyncLoader* loader = (ResourceAsyncLoader*)(args);
	ResourceManager& resourceManager = ResourceManager::get();

	ResourceLoadingInformation loadingInfo;
	while (loader->isRunning) {
		loader->loadingQueue.waitFrontAndPop(&loadingInfo);

		if (loadingInfo.resourceHandler) {
			ResourceName resourceName = resourceManager.zipFileSystem->getResourceName(loadingInfo.resourceID);
			resourceManager.loadNewResourceFromArchive(*loadingInfo.resourceHandler, resourceName, loadingInfo.resourceID);
		}
	}
}

bool ResourceAsyncLoader::init() {
	isRunning = true;
	loadingThread.init((ThreadFunction*)loadingFunction, this);

    return true;
}

void ResourceAsyncLoader::release() {
	isRunning = false;

	ResourceLoadingInformation emptyInfo{ nullptr, 0 };
	loadingQueue.push(emptyInfo);

	loadingThread.release();
}

void ResourceAsyncLoader::loadResource(ResourceID resourceID, ResourceHandler& handler) {
	ResourceLoadingInformation information{ &handler, resourceID };
	loadingQueue.push(information);
}
