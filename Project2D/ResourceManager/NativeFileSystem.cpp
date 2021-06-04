#include "NativeFileSystem.h"

#include <ResourceManager/ResourceName.h>

#include <fstream>
#include <cassert>

bool NativeFileSystem::loadRawDataBase(const ResourceName& resourceName, void* data, size_t size, std::ios_base::openmode mode) {
    std::fstream fileStream(resourceName.c_str(), mode);
    assert(fileStream.is_open());

    fileStream.read((char*)(data), size);

    fileStream.close();

    return true;
}

bool NativeFileSystem::getResourceSize(const ResourceName& resourceName, size_t& resourceSize) {
    resourceSize = (size_t)(std::filesystem::file_size(resourceName.c_str()));
    return true;
}

bool NativeFileSystem::getResourceData(const ResourceName& resourceName, void* data, size_t resourceSize) {
    return loadRawDataBase(resourceName, data, resourceSize, std::ios_base::in);
}

bool NativeFileSystem::getResourceBinaryData(const ResourceName& resourceName, void* data, size_t resourceSize) {
    return loadRawDataBase(resourceName, data, resourceSize, std::ios_base::in | std::ios_base::binary);
}
