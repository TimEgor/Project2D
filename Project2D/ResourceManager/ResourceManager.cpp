#include "ResourceManager.h"

#include <ResourceManager/ResourceCreator.h>
#include <ResourceManager/Zip/ZipFileSystem.h>
#include <ResourceManager/NativeFileSystem.h>
#include <ResourceManager/ResourceAsyncLoader.h>
#include <Utilities/WildcardMatch/WildcardMatch.h>

ResourceManager& ResourceManager::get() {
    static ResourceManager uniqueResourceManager;
    return uniqueResourceManager;
}

bool ResourceManager::init() {
    zipFileSystem = new ZipFileSystem();
    if (!zipFileSystem->init("Assets.zip")) {
        return false;
    }

    nativeFileSystem = new NativeFileSystem();
    resourceAsyncLoader = new ResourceAsyncLoader();

    if (!nativeFileSystem || !resourceAsyncLoader) {
        return false;
    }

    if (!initResourceCreators()) {
        return false;
    }

    return true;
}

void ResourceManager::release() {
    if (zipFileSystem) {
        zipFileSystem->release();
        delete zipFileSystem;
        zipFileSystem = nullptr;
    }

    if (nativeFileSystem) {
        nativeFileSystem->release();
        delete nativeFileSystem;
        nativeFileSystem = nullptr;
    }

    if (resourceAsyncLoader) {
        delete resourceAsyncLoader;
        resourceAsyncLoader = nullptr;
    }

    if (defaultResourceCreator) {
        delete defaultResourceCreator;
        defaultResourceCreator = nullptr;
    }

    for (auto& creator : resourceCreators) {
        delete creator.second;
    }
    resourceCreators = std::unordered_map<ResourceType, ResourceCreator*>();
}

bool ResourceManager::initResourceCreators() {
    defaultResourceCreator = new DefaultResourceCreator();

    return defaultResourceCreator;
}

ResourceCreator& ResourceManager::chooseResourceCreator(const ResourceName &resourceName) {
    ResourceCreator* resourceCreator = defaultResourceCreator;
    for (auto& creator : resourceCreators) {
        if (WildcardMatch(creator.second->getFilePattern(), resourceName.c_str())) {
            resourceCreator = creator.second;
            break;
        }
    }

    return *resourceCreator;
}

ResourceHandler& ResourceManager::createNewHandler(ResourceID resourceID) {
    ResourceHandler handler;
    auto newHandlerIter = resources.insert(std::make_pair(resourceID, std::move(handler)));
    return newHandlerIter.first->second;
}

ResourceReference ResourceManager::loadNewResourceFromArchive(ResourceHandler& handler, const ResourceName& resourceName, ResourceID resourceID) {
    ResourceCreator& creator = chooseResourceCreator(resourceName);
    creator.createResourceFromZipFileSystem(handler, resourceID, *zipFileSystem);

    return handler;
}

ResourceReference ResourceManager::getResourceFromArchive(const ResourceName& resourceName) {
    ResourceID resourceID = resourceName.hash();

    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        return loadNewResourceFromArchive(newHandler, resourceName, resourceID);
    }

    return findHandlerIter->second;
}

ResourceReference ResourceManager::getResourceFromArchive(ResourceID resourceID) {
    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        ResourceName resourceName = zipFileSystem->getResourceName(resourceID);

        return loadNewResourceFromArchive(newHandler, resourceName, resourceID);
    }

    return findHandlerIter->second;
}

ResourceReference ResourceManager::getResourceAsyncFromArchive(const ResourceName& resourceName) {
    ResourceID resourceID = resourceName.hash();

    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        resourceAsyncLoader->loadResource(resourceID, newHandler);

        return newHandler;
    }

    return findHandlerIter->second;
}

ResourceReference ResourceManager::getResourceAsyncFromArchive(ResourceID resourceID) {
    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        resourceAsyncLoader->loadResource(resourceID, newHandler);

        return newHandler;
    }

    return findHandlerIter->second;
}

void* ResourceManager::getRawResourceFromArchive(const ResourceName& resourceName, size_t& resourceSize) {
    return nullptr;
}

void* ResourceManager::getRawResourceFromNativeFileSystem(const ResourceName& resourceName, size_t& resourceSize) {
    return nullptr;
}
