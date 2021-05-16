#include "ResourceName.h"

#include <Utilities/HashFunction/CRC32.h>

#include <string>
#include <cassert>

void ResourceName::assign(const char* newName) {
    size_t i;
    for (i = 0; newName[i]; ++i) {
        name[i] = tolower(newName[i]);
    }
    name[i] = '\0';
}

ResourceName::ResourceName(const char* newName) {
    assign(newName);
}

ResourceName::ResourceName(const ResourceName& newName) {
    strcpy_s(name, MAX_RESOURCE_NAME_PATH, newName.name);
}

ResourceName& ResourceName::operator=(const char* newName) {
    assign(newName);
    return *this;
}

ResourceName& ResourceName::operator=(const ResourceName& newName) {
    strcpy_s(name, MAX_RESOURCE_NAME_PATH, newName.name);
    return *this;
}

ResourceName& ResourceName::operator+(const char* newName) {
    size_t strLen = strlen(name);
    size_t nameLen = length();
    if (strLen + nameLen + 1 <= MAX_RESOURCE_NAME_PATH) {
        size_t i;
        for (i = 0; newName[i]; ++i) {
            name[nameLen + i] = tolower(newName[i]);
        }
        name[nameLen + i] = '\0';
    }
    else {
        assert(false && "New name size is bigger than acceptable one");
    }

    return *this;
}

ResourceName& ResourceName::operator+(const ResourceName& newName) {
    return operator+(newName.name);
}

size_t ResourceName::length() const {
    return strlen(name);
}

ResourceID ResourceName::hash() const {
    return Crc32(name, length());
}
