#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <ResourceManager/ResourceReference.h>

class D3D11TestRenderer final {
private:

	D3D11_VIEWPORT viewport;

	ID3D11RenderTargetView* rtv;
	ID3D11RasterizerState* rastState;

	ResourceReference pixelShader;
	ResourceReference vertexShader;

	D3D11TestRenderer();

public:
	~D3D11TestRenderer() { release(); }

	static D3D11TestRenderer& get();

	bool init();
	void release();

	void render();
};