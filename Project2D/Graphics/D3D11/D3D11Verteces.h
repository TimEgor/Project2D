#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/Verteces.h>

struct D3D11SpriteVertex final {
	SpriteVertex vertex;

	static const D3D11_INPUT_ELEMENT_DESC desc[2];
};