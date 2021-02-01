#pragma once

#include <unordered_map>

enum HLSL_Type {
	HLSL_Bool,
	HLSL_Int,
	HLSL_UInt,
	HLSL_DWord,
	HLSL_Float,
	HLSL_Double,
	HLSL_Float2,
	HLSL_Float3,
	HLSL_Float4,
	HLSL_Float3x3,
	HLSL_Float4x4
};

const std::unordered_map<std::string, HLSL_Type> hlslTypes =
{
	{ "bool", HLSL_Bool },
	{ "int", HLSL_Int },
	{ "uint", HLSL_UInt },
	{ "dword", HLSL_DWord },
	{ "float", HLSL_Float },
	{ "double", HLSL_Double },
	{ "float2", HLSL_Float2 },
	{ "float3", HLSL_Float3 },
	{ "float4", HLSL_Float4 },
	{ "float3x3", HLSL_Float3x3 },
	{ "float4x4", HLSL_Float4x4 }
};

struct MaterialProperty final {
	uint32_t offset;
	HLSL_Type type;
};

class D3D11ShaderMaterialProperties final {
public:
	typedef std::unordered_map<uint32_t, MaterialProperty> PropertiesMap;
	typedef std::unordered_map<uint32_t, uint32_t> TexturesMap;

private:
	PropertiesMap properties;
	TexturesMap textures;

	size_t propertyDataSize;

public:
	D3D11ShaderMaterialProperties(void* sourceData, size_t sourceDataSize);

	const PropertiesMap& getPropertiesMap() { return properties; }
	const TexturesMap& getTextureMap() { return textures; }

	size_t getPropertyDataSize() { return propertyDataSize; }
};