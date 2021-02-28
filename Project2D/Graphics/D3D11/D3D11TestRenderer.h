#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <ResourceManager/ResourceReference.h>

class D3D11TestRenderer final {
	struct alignas(16) PerObjectTransforms final {
		DirectX::XMMATRIX worldTransformMatrix;
		DirectX::XMMATRIX viewTransformMatrix;
		DirectX::XMMATRIX projTransformMatrix;
	};

private:
	D3D11_VIEWPORT viewport;

	ID3D11RenderTargetView* rtv;
	ID3D11RasterizerState* rastState;

	ID3D11DepthStencilView* dsv;
	ID3D11Texture2D* dsBuffer;

	ID3D11SamplerState* spriteSamplerState;

	ID3D11Buffer* perObjectTransformBuffer;

	ResourceReference pixelShader;
	ResourceReference vertexShader;
	ResourceReference sprite;

	D3D11TestRenderer();

	void prepareViewTransformMatrix(DirectX::XMMATRIX& viewTransform);
	void prepareProjTransformMatrix(DirectX::XMMATRIX& projTransform);

public:
	~D3D11TestRenderer() { release(); }

	static D3D11TestRenderer& get();

	bool init();
	void release();

	void render();
};