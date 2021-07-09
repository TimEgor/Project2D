#include "D3D11Device.h"

#include <UserInterfaces/UserInterfaces.h>
#include <Graphics/D3D11/D3D11Textures.h>

#define CheckResult(result) \
    if (FAILED(result)) { \
        release(); \
        return false; \
    }

DXGI_FORMAT D3D11Device::convertFormat(Format format) {
    switch (format) {
    case FORMAT_R32G32B32A32_FLOAT:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    case FORMAT_R32G32B32A32_UINT:
        return DXGI_FORMAT_R32G32B32A32_UINT;
        break;
    case FORMAT_R32G32B32A32_SINT:
        return DXGI_FORMAT_R32G32B32A32_SINT;
        break;
    case FORMAT_R32G32B32_FLOAT:
        return DXGI_FORMAT_R32G32B32_FLOAT;
        break;
    case FORMAT_R32G32B32_UINT:
        return DXGI_FORMAT_R32G32B32_UINT;
        break;
    case FORMAT_R32G32B32_SINT:
        return DXGI_FORMAT_R32G32B32_SINT;
        break;
    case FORMAT_R16G16B16A16_FLOAT:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        break;
    case FORMAT_R16G16B16A16_UNORM:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
        break;
    case FORMAT_R16G16B16A16_UINT:
        return DXGI_FORMAT_R16G16B16A16_UINT;
        break;
    case FORMAT_R16G16B16A16_SNORM:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
        break;
    case FORMAT_R16G16B16A16_SINT:
        return DXGI_FORMAT_R16G16B16A16_SINT;
        break;
    case FORMAT_R32G32_FLOAT:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        break;
    case FORMAT_R32G32_UINT:
        return DXGI_FORMAT_R32G32_UINT;
        break;
    case FORMAT_R32G32_SINT:
        return DXGI_FORMAT_R32G32_SINT;
        break;
    case FORMAT_D32_FLOAT_S8X24_UINT:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        break;
    case FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
        break;
    case FORMAT_R10G10B10A2_UNORM:
        return DXGI_FORMAT_R10G10B10A2_UNORM;
        break;
    case FORMAT_R10G10B10A2_UINT:
        return DXGI_FORMAT_R10G10B10A2_UINT;
        break;
    case FORMAT_R11G11B10_FLOAT:
        return DXGI_FORMAT_R11G11B10_FLOAT;
        break;
    case FORMAT_R8G8B8A8_UNORM:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case FORMAT_R8G8B8A8_UNORM_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        break;
    case FORMAT_R8G8B8A8_UINT:
        return DXGI_FORMAT_R8G8B8A8_UINT;
        break;
    case FORMAT_R8G8B8A8_SNORM:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
        break;
    case FORMAT_R8G8B8A8_SINT:
        return DXGI_FORMAT_R8G8B8A8_SINT;
        break;
    case FORMAT_R16G16_FLOAT:
        return DXGI_FORMAT_R16G16_FLOAT;
        break;
    case FORMAT_R16G16_UNORM:
        return DXGI_FORMAT_R16G16_UNORM;
        break;
    case FORMAT_R16G16_UINT:
        return DXGI_FORMAT_R16G16_UINT;
        break;
    case FORMAT_R16G16_SNORM:
        return DXGI_FORMAT_R16G16_SNORM;
        break;
    case FORMAT_R16G16_SINT:
        return DXGI_FORMAT_R16G16_SINT;
        break;
    case FORMAT_D32_FLOAT:
        return DXGI_FORMAT_D32_FLOAT;
        break;
    case FORMAT_R32_FLOAT:
        return DXGI_FORMAT_R32_FLOAT;
        break;
    case FORMAT_R32_UINT:
        return DXGI_FORMAT_R32_UINT;
        break;
    case FORMAT_R32_SINT:
        return DXGI_FORMAT_R32_SINT;
        break;
    case FORMAT_D24_UNORM_S8_UINT:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
        break;
    case FORMAT_R8G8_UNORM:
        return DXGI_FORMAT_R8G8_UNORM;
        break;
    case FORMAT_R8G8_UINT:
        return DXGI_FORMAT_R8G8_UINT;
        break;
    case FORMAT_R8G8_SNORM:
        return DXGI_FORMAT_R8G8_SNORM;
        break;
    case FORMAT_R8G8_SINT:
        return DXGI_FORMAT_R8G8_SINT;
        break;
    case FORMAT_R16_FLOAT:
        return DXGI_FORMAT_R16_FLOAT;
        break;
    case FORMAT_D16_UNORM:
        return DXGI_FORMAT_D16_UNORM;
        break;
    case FORMAT_R16_UNORM:
        return DXGI_FORMAT_R16_UNORM;
        break;
    case FORMAT_R16_UINT:
        return DXGI_FORMAT_R16_UINT;
        break;
    case FORMAT_R16_SNORM:
        return DXGI_FORMAT_R16_SNORM;
        break;
    case FORMAT_R16_SINT:
        return DXGI_FORMAT_R16_SINT;
        break;
    case FORMAT_R8_UNORM:
        return DXGI_FORMAT_R8_UNORM;
        break;
    case FORMAT_R8_UINT:
        return DXGI_FORMAT_R8_UINT;
        break;
    case FORMAT_R8_SNORM:
        return DXGI_FORMAT_R8_SNORM;
        break;
    case FORMAT_R8_SINT:
        return DXGI_FORMAT_R8_SINT;
        break;
    case FORMAT_A8_UNORM:
        return DXGI_FORMAT_A8_UNORM;
        break;
    case FORMAT_R1_UNORM:
        return DXGI_FORMAT_R1_UNORM;
        break;
    case FORMAT_B4G4R4A4_UNORM:
        return DXGI_FORMAT_B4G4R4A4_UNORM;
        break;
    case FORMAT_UNKNOWN:
    default:
        return DXGI_FORMAT_UNKNOWN;
        break;
    }
}

D3D11_USAGE D3D11Device::convertUsage(Usage usage) {
    switch (usage) {
    case USAGE_IMMUTABLE:
        return D3D11_USAGE_IMMUTABLE;
        break;
    case USAGE_DYNAMIC:
        return D3D11_USAGE_DYNAMIC;
        break;
    case USAGE_STAGING:
        return D3D11_USAGE_STAGING;
        break;
    case USAGE_DEFAULT:
    default:
        return D3D11_USAGE_DEFAULT;
        break;
    }
}

D3D11_BIND_FLAG D3D11Device::convertBindFlags(Bind bindFlag) {
    switch (bindFlag) {
    case BIND_VERTEX_BUFFER:
        return D3D11_BIND_VERTEX_BUFFER;
        break;
    case BIND_INDEX_BUFFER:
        return D3D11_BIND_INDEX_BUFFER;
        break;
    case BIND_CONSTANT_BUFFER:
        return D3D11_BIND_CONSTANT_BUFFER;
        break;
    case BIND_SHADER_RESOURCE:
        return D3D11_BIND_SHADER_RESOURCE;
        break;
    case BIND_STREAM_OUTPUT:
        return D3D11_BIND_STREAM_OUTPUT;
        break;
    case BIND_RENDER_TARGET:
        return D3D11_BIND_RENDER_TARGET;
        break;
    case BIND_DEPTH_STENCIL:
        return D3D11_BIND_DEPTH_STENCIL;
        break;
    case BIND_UNORDERED_ACCESS:
        return D3D11_BIND_UNORDERED_ACCESS;
        break;
    case BIND_UNKNOWN:
    default:
        return (D3D11_BIND_FLAG)(0);
        break;
    }
}

D3D11_CPU_ACCESS_FLAG D3D11Device::convertCpuAccessFlag(CPUAccess cpuAccess) {
    switch (cpuAccess) {
    case CPU_ACCESS_WRITE: 
        return D3D11_CPU_ACCESS_WRITE;
        break;
    case CPU_ACCESS_READ:
        return D3D11_CPU_ACCESS_READ;
        break;
    case CPU_ACCESS_NULL:
    default:
        return (D3D11_CPU_ACCESS_FLAG)(0);
        break;
    }
}

bool D3D11Device::init() {
    HRESULT result;

    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

#ifdef DYNAMIC_RENDERING_LOADING
    d3d11LibHandle = LoadLibrary("d3d11.dll");

    assert(d3d11LibHandle);
    if (!d3d11LibHandle) {
        return false;
    }

    typedef HRESULT(WINAPI* PFN_D3D11_CREATE_DEVICE)(_In_opt_ IDXGIAdapter*,
        D3D_DRIVER_TYPE, HMODULE, UINT,
        _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL*,
        UINT FeatureLevels, UINT, _COM_Outptr_opt_ ID3D11Device**,
        _Out_opt_ D3D_FEATURE_LEVEL*, _COM_Outptr_opt_ ID3D11DeviceContext**);

    PFN_D3D11_CREATE_DEVICE D3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)(GetProcAddress(d3d11LibHandle, "D3D11CreateDevice"));
    assert(D3D11CreateDevice);
    if (!D3D11CreateDevice) {
        return false;
    }

    dxgiLibHandle = LoadLibrary("dxgi.dll");

    assert(dxgiLibHandle);
    if (!dxgiLibHandle) {
        return false;
    }

    typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID _riid, void** _factory);

    PFN_CREATE_DXGI_FACTORY CreateDXGIFactory = (PFN_CREATE_DXGI_FACTORY)(GetProcAddress(dxgiLibHandle, "CreateDXGIFactory"));
    assert(CreateDXGIFactory);
    if (!CreateDXGIFactory) {
        return false;
    }

#endif // DYNAMIC_RENDERING_LOADING

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

void D3D11Device::release() {
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

    if (dxgiLibHandle) {
        FreeLibrary(dxgiLibHandle);
        dxgiLibHandle = 0;
    }

    if (d3d11LibHandle) {
        FreeLibrary(d3d11LibHandle);
        d3d11LibHandle = 0;
    }
}

Texture1D_Reference D3D11Device::createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE1D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.ArraySize = 1;
    d3d11Desc.Format = convertFormat(desc.format);
    d3d11Desc.Usage = convertUsage(desc.usage);
    d3d11Desc.BindFlags = convertBindFlags(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlag(desc.cpuAccess);
    d3d11Desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData{};
    if (data) {
        subresourceData.pSysMem = data->mem;
        subresourceData.SysMemPitch = data->memPitch;
        subresourceData.SysMemSlicePitch = data->memSlicePitch;
    }

    D3D11Texture1D* newTexture = new D3D11Texture1D(*this, desc);

    device->CreateTexture1D(&d3d11Desc, &subresourceData, newTexture->getTextureNativeHandle());

	return newTexture;
}

Texture2D_Reference D3D11Device::createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE2D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.Height = desc.height;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.ArraySize = 1;
    d3d11Desc.SampleDesc.Count = desc.sampleCount;
    d3d11Desc.SampleDesc.Quality = 1;
    d3d11Desc.Format = convertFormat(desc.format);
    d3d11Desc.Usage = convertUsage(desc.usage);
    d3d11Desc.BindFlags = convertBindFlags(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlag(desc.cpuAccess);
    d3d11Desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData{};
    if (data) {
        subresourceData.pSysMem = data->mem;
        subresourceData.SysMemPitch = data->memPitch;
        subresourceData.SysMemSlicePitch = data->memSlicePitch;
    }

    D3D11Texture2D* newTexture = new D3D11Texture2D(*this, desc);

    device->CreateTexture2D(&d3d11Desc, &subresourceData, newTexture->getTextureNativeHandle());

    return newTexture;
}

Texture3D_Reference D3D11Device::createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE3D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.Height = desc.height;
    d3d11Desc.Depth = desc.depth;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.Format = convertFormat(desc.format);
    d3d11Desc.Usage = convertUsage(desc.usage);
    d3d11Desc.BindFlags = convertBindFlags(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlag(desc.cpuAccess);
    d3d11Desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData{};
    if (data) {
        subresourceData.pSysMem = data->mem;
        subresourceData.SysMemPitch = data->memPitch;
        subresourceData.SysMemSlicePitch = data->memSlicePitch;
    }

    D3D11Texture3D* newTexture = new D3D11Texture3D(*this, desc);

    device->CreateTexture3D(&d3d11Desc, &subresourceData, newTexture->getTextureNativeHandle());

    return newTexture;
}
