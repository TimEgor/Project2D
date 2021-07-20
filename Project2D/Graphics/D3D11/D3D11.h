#pragma once

#if __has_include("d3d11.h") && D3D11
#define RENDERING_D3D11
#endif // D3D11


#ifdef RENDERING_D3D11

#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#define D3D11ObjectRelease(object) \
	if (object) { \
		object->Release(); \
		object = nullptr; \
	}
#endif // RENDERING_D3D11