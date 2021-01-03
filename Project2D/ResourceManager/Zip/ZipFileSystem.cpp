#include "ZipFileSystem.h"

bool ZipFileSystem::init(const ResourceName& path) {
    return zipFile.init(path);
}

void ZipFileSystem::release() {
    zipFile.release();
}

bool ZipFileSystem::getResourceSize(const ResourceName& resourceName, size_t& resourceSize) {
    return getResourceSize(resourceName.hash(), resourceSize);
}

bool ZipFileSystem::getResourceSize(ResourceID resourceID, size_t& resourceSize) {
    return zipFile.getFileDataSize(resourceID, resourceSize);
}

bool ZipFileSystem::getResourceData(const ResourceName& resourceName, void* data, size_t resourceSize) {
    return getResourceData(resourceName.hash(), data, resourceSize);
}

bool ZipFileSystem::getResourceData(ResourceID resourceID, void* data, size_t resourceSize) {
    return zipFile.readFileData(resourceID, data, resourceSize);
}

bool ZipFileSystem::getResourceBinaryData(const ResourceName& resourceName, void* data, size_t resourceSize) {
    return getResourceBinaryData(resourceName.hash(), data, resourceSize);;
}

bool ZipFileSystem::getResourceBinaryData(ResourceID resourceID, void* data, size_t resourceSize) {
    return zipFile.readFileData(resourceID, data, resourceSize);;
}

ResourceName ZipFileSystem::getResourceName(ResourceID resourceID) {
    return zipFile.getFileName(resourceID);
}
