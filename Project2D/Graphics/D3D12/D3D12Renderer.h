#pragma once

#include <Graphics/D3D12/D3D12.h>

class D3D12Renderer final {
private:
	D3D12Renderer() = default;

public:
	~D3D12Renderer() { release(); }

	static D3D12Renderer& get();

	bool init();
	void release();

	void testDraw();
};