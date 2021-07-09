#include "ResourceManager.h"

#include <ResourceManager/ResourceCreator.h>
#include <ResourceManager/Zip/ZipFileSystem.h>
#include <ResourceManager/NativeFileSystem.h>
#include <ResourceManager/ResourceAsyncLoader.h>
#include <Utilities/WildcardMatch/WildcardMatch.h>

#include <Graphics/D3D11/Resources/D3D11MaterialResourceCreator.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResourceCreator.h>
#include <Graphics/D3D11/Resources/D3D11TextureResourceCreator.h>

#include <cassert>

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
        release();
        return false;
    }

    if (!initResourceCreators()) {
        release();
        return false;
    }

    return true;
}

void ResourceManager::release() {
    for (auto& resHandler : resources) {
        delete resHandler.second.getResource();
    }
    resources = std::unordered_map<ResourceID, ResourceHandler>();

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
    resourceCreators = std::unordered_map<ResourceCreatorType, ResourceCreator*>();
}

bool ResourceManager::initResourceCreators() {
    defaultResourceCreator = new DefaultResourceCreator();

    //resourceCreators.insert(std::make_pair(D3D11PixelShaderResourceCreatorType, new D3D11PixelShaderResourceCreator()));
    //resourceCreators.insert(std::make_pair(D3D11VertexShaderResourceCreatorType, new D3D11VertexShaderResourceCreator()));
    //resourceCreators.insert(std::make_pair(D3D11MaterialResourceCreatorType, new D3D11MaterialResourceCreator()));
    //resourceCreators.insert(std::make_pair(D3D11_PNG_TextureResourceCreatorType, new D3D11_PNG_TextureResourceCreator()));

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
    auto newValPair = resources.emplace(resourceID, resourceID);
    return newValPair.first->second;
}

ResourceReference ResourceManager::loadNewResourceFromArchive(ResourceHandler& handler, const ResourceName& resourceName, ResourceID resourceID) {
    ResourceCreator& creator = chooseResourceCreator(resourceName);
    creator.createResourceFromZipFileSystem(handler, resourceID, *zipFileSystem);

    return &handler;
}

void ResourceManager::unloadResource(ResourceHandler& handler) {
    std::unique_lock<std::mutex> locker(resMutex);

    if (handler.getRefCounter() == 0) {
        Resource* resource = handler.getResource();
        resources.erase(handler.getResourceID());

        locker.unlock();

        if (resource) {
            delete resource;
        }
    }
}

ResourceReference ResourceManager::getResourceFromArchive(const ResourceName& resourceName) {
    ResourceID resourceID = resourceName.hash();

    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end() || (findHandlerIter->second.getRefCounter() == 0)) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        return loadNewResourceFromArchive(newHandler, resourceName, resourceID);
    }

    return &(findHandlerIter->second);
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

    return &(findHandlerIter->second);
}

ResourceReference ResourceManager::getResourceAsyncFromArchive(const ResourceName& resourceName) {
    ResourceID resourceID = resourceName.hash();

    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        resourceAsyncLoader->loadResource(resourceID, newHandler);

        return &newHandler;
    }

    return &(findHandlerIter->second);
}

ResourceReference ResourceManager::getResourceAsyncFromArchive(ResourceID resourceID) {
    std::unique_lock<std::mutex> locker(resMutex);

    auto findHandlerIter = resources.find(resourceID);
    if (findHandlerIter == resources.end()) {
        ResourceHandler& newHandler = createNewHandler(resourceID);
        locker.unlock();

        resourceAsyncLoader->loadResource(resourceID, newHandler);

        return &newHandler;
    }

    return &(findHandlerIter->second);
}

void* ResourceManager::getRawResourceFromArchive(const ResourceName& resourceName, size_t& resourceSize) {
    return getRawResourceFromArchive(resourceName.hash(), resourceSize);
}

void* ResourceManager::getRawResourceFromArchive(ResourceID resourceID, size_t& resourceSize) {
    resourceSize = 0;
    bool checker = false;
    checker = zipFileSystem->getResourceSize(resourceID, resourceSize);
    assert(checker);

    void* data = new uint8_t[resourceSize];
    checker = zipFileSystem->getResourceData(resourceID, data, resourceSize);
    assert(checker);

    return data;
}

void* ResourceManager::getRawResourceFromNativeFileSystem(const ResourceName& resourceName, size_t& resourceSize) {
    return nullptr;
}
