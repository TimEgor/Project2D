#pragma once

#include <ResourceManager/Resource.h>

class TextureResource : public Resource {
protected:
	size_t width = 0;
	size_t height = 0;

public:
	TextureResource() = default;

	size_t getWidth() const { return width; }
	size_t getHeight() const { return height; }
};