#pragma once

#include <Graphics/GraphicTypes.h>
#include <Graphics/D3D11/D3D11.h>

class D3D11Device;

class D3D11Shader {
protected:
	void* sourceData = nullptr;
	size_t sourceDataSize = 0;

	void releaseSourceData() {
		if (sourceData) {
			delete[] sourceData;
		}
	}

public:
	D3D11Shader(void* sourceData, size_t sourceDataSize)
		: sourceData(sourceData), sourceDataSize(sourceDataSize) {}

	void* getSourceData() { return sourceData; }
	size_t getSourceDataSize() { return sourceDataSize; }
};

class D3D11VertexShader final : public VertexShader, public D3D11Shader {
private:
	ID3D11VertexShader* shader = nullptr;

	virtual void release() override { 
		D3D11ObjectRelease(shader);
		releaseSourceData();
	}

public:
	D3D11VertexShader(D3D11Device& device, void* sourceData, size_t sourceDataSize)
		: VertexShader(device), D3D11Shader(sourceData, sourceDataSize) {}

	ID3D11VertexShader** getShaderNativeHandle() { return &shader; }
};

class D3D11PixelShader final : public PixelShader, public D3D11Shader {
private:
	ID3D11PixelShader* shader = nullptr;

	virtual void release() override {
		D3D11ObjectRelease(shader);
		releaseSourceData();
	}

public:
	D3D11PixelShader(D3D11Device& device, void* sourceData, size_t sourceDataSize)
		: PixelShader(device), D3D11Shader(sourceData, sourceDataSize) {}

	ID3D11PixelShader** getShaderNativeHandle() { return &shader; }
};