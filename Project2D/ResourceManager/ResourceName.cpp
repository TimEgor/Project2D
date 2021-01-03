#include "ResourceName.h"

#include <Utilities/HashFunction/CRC32.h>

#include <string>

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

size_t ResourceName::length() const {
    return strlen(name);
}

ResourceID ResourceName::hash() const {
    return Crc32(name, length());
}
