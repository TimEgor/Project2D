#pragma once

#include <ResourceManager/Resource.h>

#define MAX_RESOURCE_NAME_PATH 256

typedef char ResourceNameCharType;

class ResourceName final {
private:
	ResourceNameCharType name[MAX_RESOURCE_NAME_PATH];

	void assign(const char* name);

public:
	ResourceName() : name("") {}
	ResourceName(const char* newName);
	ResourceName(const ResourceName& newName);

	ResourceName& operator=(const char* name);
	ResourceName& operator=(const ResourceName& name);
	ResourceName& operator+(const char* name);
	ResourceName& operator+(const ResourceName& name);

	const ResourceNameCharType* c_str() const { return name; }
	ResourceNameCharType* data() { return name; }
	size_t length() const;

	ResourceID hash() const;
};