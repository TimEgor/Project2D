#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11Verteces.h>

class D3D11Sprite final {
private:
	ID3D11Buffer* vertecesBuffer;
	size_t vertecesNum;

	ID3D11Buffer* indecesBuffer;
	size_t indecesNum;

	D3D11Sprite();

public:
	~D3D11Sprite() { release(); }

	static D3D11Sprite& get();

	bool init();
	void release();

	ID3D11Buffer* getVertecesBuffer() { return vertecesBuffer; }
	size_t getVertecesNum() { return vertecesNum; }

	ID3D11Buffer* getIndecesBuffer() { return indecesBuffer; }
	size_t getIndecesNum() { return indecesNum; }

	static const D3D11SpriteVertex verteces[4];
	static const uint16_t indeces[6];
};