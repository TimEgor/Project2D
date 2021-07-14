#pragma once

#include <ResourceManager/ResourceType.h>

#include <cstdint>

typedef uint32_t ResourceID;

class Resource {
public:
	virtual ~Resource() {}
	virtual ResourceType getResourceType() const = 0;
};

class DefaultResource final : public Resource {
private:
	void* data;
	size_t dataSize;

public:
	DefaultResource(void* data, size_t size)
		: data(data), dataSize(size) {}

	~DefaultResource() {
		if (data) {
			delete[] data;
		}
	}

	void* getData() { return data; }
	size_t getDataSize() { return dataSize; }

	virtual ResourceType getResourceType() const override { return DefaultResourceType; }
};