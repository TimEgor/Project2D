#pragma once

#include <Graphics/Graphics.h>

class Renderer final {
public:
	virtual ~Renderer() {}

	virtual bool createTexture1D() = 0;
};