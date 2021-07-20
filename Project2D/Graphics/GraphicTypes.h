#pragma once

#include <Multithreading/AtomicCounterObjectBase.h>
#include <Utilities/HashFunction/StdHashCombine.h>

#include <cstdint>
#include <vector>

class GraphicDevice;

enum Format {
	FORMAT_UNKNOWN,

	FORMAT_R32G32B32A32_FLOAT,
	FORMAT_R32G32B32A32_UINT,
	FORMAT_R32G32B32A32_SINT,

	FORMAT_R32G32B32_FLOAT,
	FORMAT_R32G32B32_UINT,
	FORMAT_R32G32B32_SINT,

	FORMAT_R16G16B16A16_FLOAT,
	FORMAT_R16G16B16A16_UNORM,
	FORMAT_R16G16B16A16_UINT,
	FORMAT_R16G16B16A16_SNORM,
	FORMAT_R16G16B16A16_SINT,

	FORMAT_R32G32_FLOAT,
	FORMAT_R32G32_UINT,
	FORMAT_R32G32_SINT,
	FORMAT_D32_FLOAT_S8X24_UINT,
	FORMAT_X32_TYPELESS_G8X24_UINT,

	FORMAT_R10G10B10A2_UNORM,
	FORMAT_R10G10B10A2_UINT,
	FORMAT_R11G11B10_FLOAT,

	FORMAT_R8G8B8A8_UNORM,
	FORMAT_R8G8B8A8_UNORM_SRGB,
	FORMAT_R8G8B8A8_UINT,
	FORMAT_R8G8B8A8_SNORM,
	FORMAT_R8G8B8A8_SINT,

	FORMAT_R16G16_FLOAT,
	FORMAT_R16G16_UNORM,
	FORMAT_R16G16_UINT,
	FORMAT_R16G16_SNORM,
	FORMAT_R16G16_SINT,

	FORMAT_D32_FLOAT,
	FORMAT_R32_FLOAT,
	FORMAT_R32_UINT,
	FORMAT_R32_SINT,

	FORMAT_D24_UNORM_S8_UINT,
	FORMAT_R8G8_UNORM,
	FORMAT_R8G8_UINT,
	FORMAT_R8G8_SNORM,
	FORMAT_R8G8_SINT,

	FORMAT_R16_FLOAT,
	FORMAT_D16_UNORM,
	FORMAT_R16_UNORM,
	FORMAT_R16_UINT,
	FORMAT_R16_SNORM,
	FORMAT_R16_SINT,
	FORMAT_R8_UNORM,
	FORMAT_R8_UINT,
	FORMAT_R8_SNORM,
	FORMAT_R8_SINT,
	FORMAT_A8_UNORM,
	FORMAT_R1_UNORM,

	FORMAT_B4G4R4A4_UNORM,

};

enum Usage {
	USAGE_DEFAULT,
	USAGE_IMMUTABLE,
	USAGE_DYNAMIC,
	USAGE_STAGING
};

enum Bind {
	BIND_UNKNOWN = 0,

	BIND_VERTEX_BUFFER = 1,
	BIND_INDEX_BUFFER = 2,
	BIND_CONSTANT_BUFFER = 4,
	BIND_SHADER_RESOURCE = 8,
	BIND_STREAM_OUTPUT = 16,
	BIND_RENDER_TARGET = 32,
	BIND_DEPTH_STENCIL = 64,
	BIND_UNORDERED_ACCESS = 128
};

enum CPUAccess {
	CPU_ACCESS_NULL = 0,

	CPU_ACCESS_WRITE = 1,
	CPU_ACCESS_READ = 2
};

enum ShaderType {
	VERTEX_SHADER_TYPE,
	PIXEL_SHADER_TYPE
};

enum InputLayoutElementType {
	INPUT_LAYOUT_ELEMENT_TYPE_UNKNOWN,

	INPUT_LAYOUT_ELEMENT_TYPE_INT8,
	INPUT_LAYOUT_ELEMENT_TYPE_INT16,
	INPUT_LAYOUT_ELEMENT_TYPE_INT32,
	INPUT_LAYOUT_ELEMENT_TYPE_UINT8,
	INPUT_LAYOUT_ELEMENT_TYPE_UINT16,
	INPUT_LAYOUT_ELEMENT_TYPE_UINT32,
	INPUT_LAYOUT_ELEMENT_TYPE_FLOAT16,
	INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32
};

struct SubresourceData {
	const void* mem = nullptr;
	uint32_t memPitch = 0;
	uint32_t memSlicePitch = 0;
};

struct Texture1DDesc final {
	uint32_t width = 0;
	Format format = FORMAT_UNKNOWN;
	Usage usage = USAGE_DEFAULT;
	uint32_t bind = BIND_UNKNOWN;
	uint32_t cpuAccess = CPU_ACCESS_NULL;
};

struct Texture2DDesc final {
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t sampleCount = 1;
	Format format = FORMAT_UNKNOWN;
	Usage usage = USAGE_DEFAULT;
	uint32_t bind = BIND_UNKNOWN;
	uint32_t cpuAccess = CPU_ACCESS_NULL;
};

struct Texture3DDesc final {
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t depth = 0;
	uint32_t sampleCount = 1;
	Format format = FORMAT_UNKNOWN;
	Usage usage = USAGE_DEFAULT;
	uint32_t bind = BIND_UNKNOWN;
	uint32_t cpuAccess = CPU_ACCESS_NULL;
};

struct GPUBufferDesc final {
	uint32_t byteSize = 0;
	Usage usage = USAGE_DEFAULT;
	uint32_t bind = BIND_UNKNOWN;
	uint32_t cpuAccess = CPU_ACCESS_NULL;
	uint32_t structureByteStride = 0;
};

struct GPUObject : public AtomicCounterObjectBase {
private:
	GraphicDevice& device;

	virtual void selfDestroy() override {
		release();
		delete this;
	}

protected:
	virtual void release() = 0;

public:
	GPUObject(GraphicDevice& device)
		: device(device) {}

	GraphicDevice& getDeviceObject() const { return device; }
};

class Texture1D : public GPUObject {
protected:
	Texture1DDesc desc;

public:
	Texture1D(GraphicDevice& device, const Texture1DDesc& desc)
		: GPUObject(device), desc(desc) {}

	const Texture1DDesc& getDesc() const { return desc; }
};

class Texture2D : public GPUObject {
protected:
	Texture2DDesc desc;

public:
	Texture2D(GraphicDevice& device, const Texture2DDesc& desc)
		: GPUObject(device), desc(desc) {}

	const Texture2DDesc& getDesc() const { return desc; }
};

class Texture3D : public GPUObject {
protected:
	Texture3DDesc desc;

public:
	Texture3D(GraphicDevice& device, const Texture3DDesc& desc)
		: GPUObject(device), desc(desc) {}

	const Texture3DDesc& getDesc() const { return desc; }
};

class GPUBuffer : public GPUObject {
protected:
	GPUBufferDesc desc;

public:
	GPUBuffer(GraphicDevice& device, const GPUBufferDesc& desc)
		: GPUObject(device), desc(desc) {}

	const GPUBufferDesc& getDesc() const { return desc; }
};

class VertexShader : public GPUObject {
public:
	VertexShader(GraphicDevice& device)
		: GPUObject(device) {}

	ShaderType getType() const { return VERTEX_SHADER_TYPE; }
};

class PixelShader : public GPUObject {
public:
	PixelShader(GraphicDevice& device)
		: GPUObject(device) {}

	ShaderType getType() const { return PIXEL_SHADER_TYPE; }
};

struct InputLayoutElement final {
	const char* name = "ATTRIB";
	uint32_t index = 0;
	uint32_t slot = 0;
	InputLayoutElementType type;
	uint32_t componentsNum = 0;
	uint32_t offset = 0;
	uint32_t stride = 0;

	size_t getID() const {
		size_t res = 0;

		hash_combine(res, name);
		hash_combine(res, index);
		hash_combine(res, slot);
		hash_combine(res, type);
		hash_combine(res, componentsNum);
		hash_combine(res, offset);
		hash_combine(res, stride);

		return res;
	}
};

class InputLayoutDesc final {
private:
	std::vector<InputLayoutElement> elements;

public:
	InputLayoutDesc() = default;

	const std::vector<InputLayoutElement>& getElements() const { return elements; }

	void add(const InputLayoutElement& element) { elements.push_back(element); }
	void reserve(size_t size) { elements.reserve(size); }
	void clear() { elements.clear(); }

	size_t getID() const {
		size_t res = 0;

		for (const auto& element : elements) {
			hash_combine(res, element.getID());
		}

		return res;
	}
};

class InputLayout : public GPUObject {
	size_t id;

public:
	InputLayout(GraphicDevice& device, size_t id)
		: GPUObject(device), id(id) {}

	size_t getID() const { return id; }
};

typedef AtomicCounterObjetcBaseReference<Texture1D> Texture1DReference;
typedef AtomicCounterObjetcBaseReference<Texture2D> Texture2DReference;
typedef AtomicCounterObjetcBaseReference<Texture3D> Texture3DReference;
typedef AtomicCounterObjetcBaseReference<GPUBuffer> GPUBufferReference;
typedef AtomicCounterObjetcBaseReference<VertexShader> VertexShaderReference;
typedef AtomicCounterObjetcBaseReference<PixelShader> PixelShaderReference;
typedef AtomicCounterObjetcBaseReference<InputLayout> InputLayerReference;