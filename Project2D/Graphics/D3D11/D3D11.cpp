#include "D3D11.h"

#include <UserInterfaces/UserInterfaces.h>

#define CheckResult(result) \
    if (FAILED(result)) { \
        release(); \
        return false; \
    }

D3D11::D3D11() : device(nullptr), deviceContext(nullptr),
    dxgiFactory(nullptr), swapChain(nullptr) {}

D3D11& D3D11::get() {
    static D3D11 uniqueD3D11;
    return uniqueD3D11;
}

bool D3D11::init() {
    HRESULT result;

    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG


    D3D_FEATURE_LEVEL featureLevel;
    result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flags, nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext);
    CheckResult(result);

    result = CreateDXGIFactory(__uuidof(dxgiFactory), (void**)(&dxgiFactory));
    CheckResult(result);

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 800;
    swapChainDesc.BufferDesc.Height = 800;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = true;
    swapChainDesc.OutputWindow = UserInterfaces::get().getMainWindow();

    result = dxgiFactory->CreateSwapChain(device, &swapChainDesc, &swapChain);
    CheckResult(result);

    return true;
}

void D3D11::release() {
    D3D11ObjectRelease(swapChain);
    D3D11ObjectRelease(dxgiFactory);
    D3D11ObjectRelease(deviceContext);

#ifdef _DEBUG
    if (device) {
        ID3D11Debug* debug;
        device->QueryInterface(__uuidof(debug), (void**)(&debug));
        debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        D3D11ObjectRelease(debug);
    }
#endif // _DEBUG

    D3D11ObjectRelease(device);
}
