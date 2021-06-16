#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/RenderingOrder.h>

class D3D11SpriteBatch final {
private:
	ID3D11Buffer* vertecesBuffer;
	ID3D11Buffer* indecesBuffer;
	size_t spriteCount;
	size_t capacity;

public:
	D3D11SpriteBatch() : vertecesBuffer(nullptr), indecesBuffer(nullptr), spriteCount(0) {}
	~D3D11SpriteBatch() { release(); }

	bool init(size_t count);
	void release();

	//size_t buildData(const std::vector<RenderingOrderNode*>& nodes, size_t currentIndex, RenderingOrderType type);

	size_t getSpriteCount() const { return spriteCount; }

	ID3D11Buffer* getVertecesBuffer() const { return vertecesBuffer; }
	ID3D11Buffer* getIndecesBuffer() const { return indecesBuffer; }

	bool isInit() const { return vertecesBuffer || indecesBuffer; }
};