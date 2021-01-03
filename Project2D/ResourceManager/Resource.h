#pragma once

#include <ResourceManager/ResourceType.h>

#include <cstdint>

typedef uint32_t ResourceID;

class Resource {
public:
	virtual ~Resource() {}
	virtual ResourceType getResourceType() = 0;
};

class DefaultResource final : public Resource {
private:
	void* data;
	size_t dataSize;

public:
	DefaultResource(void* data, size_t size)
		: data(data), dataSize(size) {}

	void* getData() { return data; }
	size_t getDataSize() { return dataSize; }

	virtual ResourceType getResourceType() override { return DefaultResourceType; }
};