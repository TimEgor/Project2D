#include "D3D11Device.h"

#include <UserInterfaces/UserInterfaces.h>
#include <Graphics/D3D11/D3D11Textures.h>
#include <Graphics/D3D11/D3D11Buffer.h>
#include <Graphics/D3D11/D3D11Shaders.h>
#include <Graphics/D3D11/D3D11PipelineState.h>
#include <Graphics/D3D11/D3D11InputLayout.h>
#include <Graphics/D3D11/WICTextureLoader11.h>

#define CheckResult(result) \
    if (FAILED(result)) { \
        release(); \
        return false; \
    }

DXGI_FORMAT D3D11Device::convertFormatToD3D11(Format format) {
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

Format D3D11Device::convertFormatFromD3D11(DXGI_FORMAT format) {
    switch (format) {
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return FORMAT_R32G32B32A32_FLOAT;
        break;
    case DXGI_FORMAT_R32G32B32A32_UINT:
        return FORMAT_R32G32B32A32_UINT;
        break;
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return FORMAT_R32G32B32A32_SINT;
        break;
    case DXGI_FORMAT_R32G32B32_FLOAT:
        return FORMAT_R32G32B32_FLOAT;
        break;
    case DXGI_FORMAT_R32G32B32_UINT:
        return FORMAT_R32G32B32_UINT;
        break;
    case DXGI_FORMAT_R32G32B32_SINT:
        return FORMAT_R32G32B32_SINT;
        break;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
        return FORMAT_R16G16B16A16_FLOAT;
        break;
    case DXGI_FORMAT_R16G16B16A16_UNORM:
        return FORMAT_R16G16B16A16_UNORM;
        break;
    case DXGI_FORMAT_R16G16B16A16_UINT:
        return FORMAT_R16G16B16A16_UINT;
        break;
    case DXGI_FORMAT_R16G16B16A16_SNORM:
        return FORMAT_R16G16B16A16_SNORM;
        break;
    case DXGI_FORMAT_R16G16B16A16_SINT:
        return FORMAT_R16G16B16A16_SINT;
        break;
    case DXGI_FORMAT_R32G32_FLOAT:
        return FORMAT_R32G32_FLOAT;
        break;
    case DXGI_FORMAT_R32G32_UINT:
        return FORMAT_R32G32_UINT;
        break;
    case DXGI_FORMAT_R32G32_SINT:
        return FORMAT_R32G32_SINT;
        break;
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        return FORMAT_D32_FLOAT_S8X24_UINT;
        break;
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return FORMAT_X32_TYPELESS_G8X24_UINT;
        break;
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return FORMAT_R8G8B8A8_UNORM;
        break;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return FORMAT_R8G8B8A8_UNORM_SRGB;
        break;
    case DXGI_FORMAT_R8G8B8A8_UINT:
        return FORMAT_R8G8B8A8_UINT;
        break;
    case DXGI_FORMAT_R8G8B8A8_SNORM:
        return FORMAT_R8G8B8A8_SNORM;
        break;
    case DXGI_FORMAT_R8G8B8A8_SINT:
        return FORMAT_R8G8B8A8_SINT;
        break;
    case DXGI_FORMAT_R16G16_FLOAT:
        return FORMAT_R16G16_FLOAT;
        break;
    case DXGI_FORMAT_R16G16_UNORM:
        return FORMAT_R16G16_UNORM;
        break;
    case DXGI_FORMAT_R16G16_UINT:
        return FORMAT_R16G16_UINT;
        break;
    case DXGI_FORMAT_R16G16_SNORM:
        return FORMAT_R16G16_SNORM;
        break;
    case DXGI_FORMAT_R16G16_SINT:
        return FORMAT_R16G16_SINT;
        break;
    case DXGI_FORMAT_D32_FLOAT:
        return FORMAT_D32_FLOAT;
        break;
    case DXGI_FORMAT_R32_FLOAT:
        return FORMAT_R32_FLOAT;
        break;
    case DXGI_FORMAT_R32_UINT:
        return FORMAT_R32_UINT;
        break;
    case DXGI_FORMAT_R32_SINT:
        return FORMAT_R32_SINT;
        break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return FORMAT_D24_UNORM_S8_UINT;
        break;
    case DXGI_FORMAT_R8G8_UNORM:
        return FORMAT_R8G8_UNORM;
        break;
    case DXGI_FORMAT_R8G8_UINT:
        return FORMAT_R8G8_UINT;
        break;
    case DXGI_FORMAT_R8G8_SNORM:
        return FORMAT_R8G8_SNORM;
        break;
    case DXGI_FORMAT_R8G8_SINT:
        return FORMAT_R8G8_SINT;
        break;
    case DXGI_FORMAT_R16_FLOAT:
        return FORMAT_R16_FLOAT;
        break;
    case DXGI_FORMAT_D16_UNORM:
        return FORMAT_D16_UNORM;
        break;
    case DXGI_FORMAT_R16_UNORM:
        return FORMAT_R16_UNORM;
        break;
    case DXGI_FORMAT_R16_UINT:
        return FORMAT_R16_UINT;
        break;
    case DXGI_FORMAT_R16_SNORM:
        return FORMAT_R16_SNORM;
        break;
    case DXGI_FORMAT_R16_SINT:
        return FORMAT_R16_SINT;
        break;
    case DXGI_FORMAT_R8_UNORM:
        return FORMAT_R8_UNORM;
        break;
    case DXGI_FORMAT_R8_UINT:
        return FORMAT_R8_UINT;
        break;
    case DXGI_FORMAT_R8_SNORM:
        return FORMAT_R8_SNORM;
        break;
    case DXGI_FORMAT_R8_SINT:
        return FORMAT_R8_SINT;
        break;
    case DXGI_FORMAT_A8_UNORM:
        return FORMAT_A8_UNORM;
        break;
    case DXGI_FORMAT_R1_UNORM:
        return FORMAT_R1_UNORM;
        break;
    case DXGI_FORMAT_B4G4R4A4_UNORM:
        return FORMAT_B4G4R4A4_UNORM;
        break;
    case DXGI_FORMAT_UNKNOWN:
    default:
        return FORMAT_UNKNOWN;
        break;
    }
}

D3D11_USAGE D3D11Device::convertUsageToD3D11(Usage usage) {
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

Usage D3D11Device::convertUsageFromD3D11(D3D11_USAGE usage) {
    switch (usage) {
    case D3D11_USAGE_IMMUTABLE:
        return USAGE_IMMUTABLE;
        break;
    case D3D11_USAGE_DYNAMIC:
        return USAGE_DYNAMIC;
        break;
    case D3D11_USAGE_STAGING:
        return USAGE_STAGING;
        break;
    case D3D11_USAGE_DEFAULT:
    default:
        return USAGE_DEFAULT;
        break;
    }
}

D3D11_BIND_FLAG D3D11Device::convertBindFlagToD3D11(Bind bindFlag) {
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

Bind D3D11Device::convertBindFlagFromD3D11(D3D11_BIND_FLAG bindFlag) {
    switch (bindFlag) {
    case D3D11_BIND_VERTEX_BUFFER:
        return BIND_VERTEX_BUFFER;
        break;
    case D3D11_BIND_INDEX_BUFFER:
        return BIND_INDEX_BUFFER;
        break;
    case D3D11_BIND_CONSTANT_BUFFER:
        return BIND_CONSTANT_BUFFER;
        break;
    case D3D11_BIND_SHADER_RESOURCE:
        return BIND_SHADER_RESOURCE;
        break;
    case D3D11_BIND_STREAM_OUTPUT:
        return BIND_STREAM_OUTPUT;
        break;
    case D3D11_BIND_RENDER_TARGET:
        return BIND_RENDER_TARGET;
        break;
    case D3D11_BIND_DEPTH_STENCIL:
        return BIND_DEPTH_STENCIL;
        break;
    case D3D11_BIND_UNORDERED_ACCESS:
        return BIND_UNORDERED_ACCESS;
        break;
    default:
        return BIND_UNKNOWN;
        break;
    }
}

D3D11_CPU_ACCESS_FLAG D3D11Device::convertCpuAccessFlagToD3D11(CPUAccess cpuAccess) {
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

CPUAccess D3D11Device::convertCpuAccessFlagFromD3D11(D3D11_CPU_ACCESS_FLAG cpuAccess) {
    switch (cpuAccess) {
    case D3D11_CPU_ACCESS_WRITE:
        return CPU_ACCESS_WRITE;
        break;
    case D3D11_CPU_ACCESS_READ:
        return CPU_ACCESS_READ;
        break;
    default:
        return CPU_ACCESS_NULL;
        break;
    }
}

uint32_t D3D11Device::convertBindFlagsToD3D11(uint32_t bindFlags) {
    uint32_t flags = 0;

    if (bindFlags & BIND_VERTEX_BUFFER) {
        flags |= D3D11_BIND_VERTEX_BUFFER;
    }
    if (bindFlags & BIND_INDEX_BUFFER) {
        flags |= D3D11_BIND_INDEX_BUFFER;
    }
    if (bindFlags & BIND_CONSTANT_BUFFER) {
        flags |= D3D11_BIND_CONSTANT_BUFFER;
    }
    if (bindFlags & BIND_SHADER_RESOURCE) {
        flags |= D3D11_BIND_SHADER_RESOURCE;
    }
    if (bindFlags & BIND_STREAM_OUTPUT) {
        flags |= D3D11_BIND_STREAM_OUTPUT;
    }
    if (bindFlags & BIND_RENDER_TARGET) {
        flags |= D3D11_BIND_RENDER_TARGET;
    }
    if (bindFlags & BIND_DEPTH_STENCIL) {
        flags |= D3D11_BIND_DEPTH_STENCIL;
    }
    if (bindFlags & BIND_UNORDERED_ACCESS) {
        flags |= D3D11_BIND_UNORDERED_ACCESS;
    }

    return flags;
}

uint32_t D3D11Device::convertBindFlagsFromD3D11(uint32_t bindFlags) {
    uint32_t flags = 0;

    if (bindFlags & D3D11_BIND_VERTEX_BUFFER) {
        flags |= BIND_VERTEX_BUFFER;
    }
    if (bindFlags & D3D11_BIND_INDEX_BUFFER) {
        flags |= BIND_INDEX_BUFFER;
    }
    if (bindFlags & D3D11_BIND_CONSTANT_BUFFER) {
        flags |= BIND_CONSTANT_BUFFER;
    }
    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
        flags |= BIND_SHADER_RESOURCE;
    }
    if (bindFlags & D3D11_BIND_STREAM_OUTPUT) {
        flags |= BIND_STREAM_OUTPUT;
    }
    if (bindFlags & D3D11_BIND_RENDER_TARGET) {
        flags |= BIND_RENDER_TARGET;
    }
    if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {
        flags |= BIND_DEPTH_STENCIL;
    }
    if (bindFlags & D3D11_BIND_UNORDERED_ACCESS) {
        flags |= BIND_UNORDERED_ACCESS;
    }

    return flags;
}

uint32_t D3D11Device::convertCpuAccessFlagsToD3D11(uint32_t cpuAccessFlags) {
    uint32_t flags = 0;

    if (cpuAccessFlags & CPU_ACCESS_READ) {
        cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
    }
    if (cpuAccessFlags & CPU_ACCESS_WRITE) {
        cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
    }

    return flags;
}

uint32_t D3D11Device::convertCpuAccessFlagsFromD3D11(uint32_t cpuAccessFlags) {
    uint32_t flags = 0;

    if (cpuAccessFlags & D3D11_CPU_ACCESS_READ) {
        cpuAccessFlags |= CPU_ACCESS_READ;
    }
    if (cpuAccessFlags & D3D11_CPU_ACCESS_WRITE) {
        cpuAccessFlags |= CPU_ACCESS_WRITE;
    }

    return flags;
}

DXGI_FORMAT D3D11Device::convertInputLayoutTypeToDXGI(InputLayoutElementType type, uint32_t componentsNum) {
    switch (type) {
    case INPUT_LAYOUT_ELEMENT_TYPE_FLOAT16:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R16_FLOAT;
                break;
            case 2:
                return DXGI_FORMAT_R16G16_FLOAT;
                break;
            case 4: 
                return DXGI_FORMAT_R16G16B16A16_FLOAT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R32_FLOAT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_FLOAT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_INT32:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R32_SINT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_SINT;
                break;
            case 3: return DXGI_FORMAT_R32G32B32_SINT;
                break;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_SINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_UINT32:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R32_UINT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_UINT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_UINT;
                break;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_UINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_INT16:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R16_SINT;
                break;
            case 2:
                return DXGI_FORMAT_R16G16_SINT;
                break;
            case 4:
                return DXGI_FORMAT_R16G16B16A16_SINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_UINT16:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R16_UINT;
                break;
            case 2:
                return DXGI_FORMAT_R16G16_UINT;
                break;
            case 4:
                return DXGI_FORMAT_R16G16B16A16_UINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_INT8:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R8_SINT;
                break;
            case 2:
                return DXGI_FORMAT_R8G8_SINT;
                break;
            case 4:
                return DXGI_FORMAT_R8G8B8A8_SINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    case INPUT_LAYOUT_ELEMENT_TYPE_UINT8:
    {
        switch (componentsNum) {
            case 1:
                return DXGI_FORMAT_R8_UINT;
                break;
            case 2:
                return DXGI_FORMAT_R8G8_UINT;
                break;
            case 4:
                return DXGI_FORMAT_R8G8B8A8_UINT;
                break;
            default:
                return DXGI_FORMAT_UNKNOWN;
                break;
        }
    }

    default:
        return DXGI_FORMAT_UNKNOWN;
        break;
    }
}

void D3D11Device::convertInputLayoutDescToD3D11(const std::vector<InputLayoutElement>& desc, std::vector<D3D11_INPUT_ELEMENT_DESC>& result) {
    result.reserve(desc.size());

    for (const InputLayoutElement& element : desc) {
        D3D11_INPUT_ELEMENT_DESC newD3D11Desc{};
        newD3D11Desc.SemanticName = element.name;
        newD3D11Desc.SemanticIndex = element.index;
        newD3D11Desc.Format = convertInputLayoutTypeToDXGI(element.type, element.componentsNum);
        newD3D11Desc.InputSlot = element.slot;
        newD3D11Desc.AlignedByteOffset = element.offset;
        newD3D11Desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        newD3D11Desc.InstanceDataStepRate = 0;

        result.push_back(newD3D11Desc);
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

Texture1DReference D3D11Device::createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE1D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.ArraySize = 1;
    d3d11Desc.Format = convertFormatToD3D11(desc.format);
    d3d11Desc.Usage = convertUsageToD3D11(desc.usage);
    d3d11Desc.BindFlags = convertBindFlagsToD3D11(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlagsToD3D11(desc.cpuAccess);
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

Texture2DReference D3D11Device::createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE2D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.Height = desc.height;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.ArraySize = 1;
    d3d11Desc.SampleDesc.Count = desc.sampleCount;
    d3d11Desc.SampleDesc.Quality = 1;
    d3d11Desc.Format = convertFormatToD3D11(desc.format);
    d3d11Desc.Usage = convertUsageToD3D11(desc.usage);
    d3d11Desc.BindFlags = convertBindFlagsToD3D11(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlagsToD3D11(desc.cpuAccess);
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

Texture3DReference D3D11Device::createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_TEXTURE3D_DESC d3d11Desc{};
    d3d11Desc.Width = desc.width;
    d3d11Desc.Height = desc.height;
    d3d11Desc.Depth = desc.depth;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.Format = convertFormatToD3D11(desc.format);
    d3d11Desc.Usage = convertUsageToD3D11(desc.usage);
    d3d11Desc.BindFlags = convertBindFlagsToD3D11(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlagsToD3D11(desc.cpuAccess);
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

Texture2DReference D3D11Device::createTexture2DFromMemory(const SubresourceData* data) {
    assert(device);

    Texture2DDesc desc{};
    D3D11Texture2D* newTexture = new D3D11Texture2D(*this, desc);

    ID3D11Resource** nativeTextureObject = (ID3D11Resource**)(newTexture->getTextureNativeHandle());
    DirectX::CreateWICTextureFromMemory(device, (const uint8_t*)(data->mem), data->memPitch, nativeTextureObject, nullptr);

    D3D11_TEXTURE2D_DESC d3d11Desc{};
    (*newTexture->getTextureNativeHandle())->GetDesc(&d3d11Desc);

    newTexture->desc.width = d3d11Desc.Width;
    newTexture->desc.height = d3d11Desc.Height;
    newTexture->desc.format = convertFormatFromD3D11(d3d11Desc.Format);
    newTexture->desc.usage = convertUsageFromD3D11(d3d11Desc.Usage);
    newTexture->desc.bind = convertBindFlagsFromD3D11(d3d11Desc.BindFlags);
    newTexture->desc.cpuAccess = convertCpuAccessFlagsFromD3D11(d3d11Desc.CPUAccessFlags);

    return newTexture;
}

GPUBufferReference D3D11Device::createGPUBuffer(const GPUBufferDesc& desc, const SubresourceData* data) {
    assert(device);

    D3D11_BUFFER_DESC d3d11Desc{};
    d3d11Desc.ByteWidth = desc.byteSize;
    d3d11Desc.StructureByteStride = desc.structureByteStride;
    d3d11Desc.Usage = convertUsageToD3D11(desc.usage);
    d3d11Desc.BindFlags = convertBindFlagsToD3D11(desc.bind);
    d3d11Desc.CPUAccessFlags = convertCpuAccessFlagsToD3D11(desc.cpuAccess);
    d3d11Desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData{};
    if (data) {
        subresourceData.pSysMem = data->mem;
        subresourceData.SysMemPitch = data->memPitch;
        subresourceData.SysMemSlicePitch = data->memSlicePitch;
    }

    D3D11GpuBuffer* newBuffer = new D3D11GpuBuffer(*this, desc);

    device->CreateBuffer(&d3d11Desc, &subresourceData, newBuffer->getBufferNativeHandle());

    return newBuffer;
}

VertexShaderReference D3D11Device::createVertexShaderFromCompiledCode(void* data, size_t size) {
    void* sourceData = new uint8_t[size];
    memcpy(sourceData, data, size);

    D3D11VertexShader* newShader = new D3D11VertexShader(*this, sourceData, size);

    device->CreateVertexShader(data, size, nullptr, newShader->getShaderNativeHandle());

    return newShader;
}

PixelShaderReference D3D11Device::createPixelShaderFromCompiledCode(void* data, size_t size) {
    void* sourceData = new uint8_t[size];
    memcpy(sourceData, data, size);

    D3D11PixelShader* newShader = new D3D11PixelShader(*this, sourceData, size);

    device->CreatePixelShader(data, size, nullptr, newShader->getShaderNativeHandle());

    return newShader;
}

VertexShaderReference D3D11Device::createVertexShaderFromStrSource(void* data, size_t size) {
    return VertexShaderReference();
}

PixelShaderReference D3D11Device::createPixelShaderFromStrSource(void* data, size_t size) {
    return PixelShaderReference();
}

InputLayerReference D3D11Device::createInputLayout(const InputLayoutDesc& desc, VertexShaderReference vertexShader) {
    D3D11InputLayout* inputLayer = nullptr;

    size_t id = desc.getID();

    auto layoutFindIter = layouts.find(id);
    if (layoutFindIter == layouts.end()) {
        inputLayer = new D3D11InputLayout(*this, id);

        const std::vector<InputLayoutElement>& descElements = desc.getElements();
        std::vector<D3D11_INPUT_ELEMENT_DESC> d3d11Desc;
        convertInputLayoutDescToD3D11(descElements, d3d11Desc);

        D3D11VertexShader* vertexShaderObject = (D3D11VertexShader*)(vertexShader.getObject());

        device->CreateInputLayout(d3d11Desc.data(), d3d11Desc.size(), vertexShaderObject->getSourceData(), vertexShaderObject->getSourceDataSize(), inputLayer->getInputLayerNativeHandle());

        layouts.insert(std::make_pair(id, inputLayer));
    }
    else {
        inputLayer = layoutFindIter->second;
    }

    return inputLayer;
}

PipelineStateReference D3D11Device::createPipelineState(const PipelineStateDesc& desc) {
    D3D11PipelineState* pipeline = new D3D11PipelineState(*this, desc);
    return pipeline;
}
