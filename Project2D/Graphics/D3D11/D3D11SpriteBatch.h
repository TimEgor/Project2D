#pragma once

#include <Graphics\D3D11\D3D11.h>

struct RenderingOrderNode;

class D3D11SpriteBatch final {
private:
	ID3D11Buffer* vertecesBuffer;
	ID3D11Buffer* indecesBuffer;
	size_t spriteCount;

public:
	D3D11SpriteBatch() : vertecesBuffer(nullptr), indecesBuffer(nullptr), spriteCount(0) {}
	~D3D11SpriteBatch() { release(); }

	bool init(size_t count);
	void release();

	void buildData(RenderingOrderNode* node, size_t count);

	ID3D11Buffer* getVertecesBuffer() { return vertecesBuffer; }
	ID3D11Buffer* getIndecesBuffer() { return indecesBuffer; }
};