#include "D3D11Verteces.h"

const D3D11_INPUT_ELEMENT_DESC D3D11SpriteVertex::desc[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0}
};