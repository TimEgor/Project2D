#pragma once

#include <Graphics/D3D11/D3D11.h>

class D3D11Renderer final {
private:

	ID3D11RenderTargetView* rtv;

	D3D11Renderer();

public:
	static D3D11Renderer& get();

	bool init();
	void release();

	void testDraw();
};