#pragma once

#include <Graphics/D3D11/D3D11.h>

struct D3D11SpriteVertex final {
	float posX, posY;
	float texU, texV;

	static const D3D11_INPUT_ELEMENT_DESC desc[2];
};