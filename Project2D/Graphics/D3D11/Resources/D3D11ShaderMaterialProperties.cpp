#include "D3D11ShaderMaterialProperties.h"

#include <Graphics/D3D11/D3D11.h>
#include <ResourceManager/ResourceName.h>

#include <cassert>

inline HLSL_Type getHLSL_Type(const char* str) {
	auto findTypeIter = hlslTypes.find(str);
	assert(findTypeIter != hlslTypes.end());

	return findTypeIter->second;
}

D3D11ShaderMaterialProperties::D3D11ShaderMaterialProperties(void* sourceData, size_t sourceDataSize) {
	assert(sourceData);

	ID3D11ShaderReflection* reflector = nullptr;
	D3D11_SHADER_DESC shaderDesc;
	ID3D11ShaderReflectionConstantBuffer* materialBuffer = nullptr;
	D3D11_SHADER_BUFFER_DESC materialConstantBufferDesc;
	D3D11_SHADER_TYPE_DESC varTypeDesc;
	D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;

	D3DReflect(sourceData, sourceDataSize, __uuidof(reflector) , (void**)(&reflector));
	materialBuffer = reflector->GetConstantBufferByName("MaterialProperties");

	propertyDataSize = 0;

	size_t varsNum = 0;
	if (!FAILED(materialBuffer->GetDesc(&materialConstantBufferDesc))) {
		propertyDataSize = materialConstantBufferDesc.Size;
		varsNum = materialConstantBufferDesc.Variables;
	}

	reflector->GetDesc(&shaderDesc);

	for (size_t i = 0; i < shaderDesc.BoundResources; ++i) {
		reflector->GetResourceBindingDesc(i, &inputBindDesc);
		if (inputBindDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE) {
			textures.insert(std::make_pair(ResourceName(inputBindDesc.Name).hash(), inputBindDesc.BindPoint));
		}
	}

	if (varsNum != 0) {
		properties.reserve(varsNum);

		ID3D11ShaderReflectionVariable* materialVar;
		D3D11_SHADER_VARIABLE_DESC varDesc;

		uint32_t propertyID;
		uint32_t offset;

		for (UINT i = 0; i < varsNum; ++i) {
			materialVar = materialBuffer->GetVariableByIndex(i);
			materialVar->GetDesc(&varDesc);

			offset = varDesc.StartOffset;
			propertyID = ResourceName(varDesc.Name).hash();

			materialVar->GetType()->GetDesc(&varTypeDesc);
			HLSL_Type type = getHLSL_Type(varTypeDesc.Name);

			properties.insert(std::make_pair(propertyID, MaterialProperty{ offset, type }));
		}
	}

	D3D11ObjectRelease(reflector);
}
