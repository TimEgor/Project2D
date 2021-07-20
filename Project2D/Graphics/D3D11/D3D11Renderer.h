#pragma once

#include <Graphics/Renderer.h>

class D3D11Renderer final : public Renderer {
private:

public:
	virtual void beginDrawing() override;
	virtual void endRendering() override;

	virtual void testRender() override;
};