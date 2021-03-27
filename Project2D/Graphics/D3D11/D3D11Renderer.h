#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/RenderingData.h>

struct alignas(16) PerObjectTransforms final {
	DirectX::XMMATRIX worldTransformMatrix;
	DirectX::XMMATRIX viewTransformMatrix;
	DirectX::XMMATRIX projTransformMatrix;
};

class D3D11Renderer final {
private:
	D3D11_VIEWPORT viewport;

	ID3D11RenderTargetView* renderTargetView;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11RasterizerState* rasterizerState;

	ID3D11Buffer* perObjectTransformBuffer;
	ID3D11SamplerState* spriteSampler;

	ResourceID currentMaterialID;
	ResourceID currentSpriteID;

	D3D11Renderer();

	void prepareViewTransformMatrix(DirectX::XMMATRIX& matrix);
	void prepareSceneProjTransformMatrix(DirectX::XMMATRIX& matrix);
	void prepareCanvasProjTransformMatrix(DirectX::XMMATRIX& matrix);

	void changeMaterial(ResourceReference materialResource, ID3D11DeviceContext* context);
	void changeSprite(ResourceReference spriteResource, ID3D11DeviceContext* context);

public:
	~D3D11Renderer() { release(); }

	static D3D11Renderer& get();

	bool init();
	void release();

	void beginDrawing();
	void endDrawing();

	void draw(SceneSpaceRenderingData data);
	void draw(CanvasSpaceRenderingData data);
};