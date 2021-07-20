#pragma once

#include <Graphics/GraphicTypes.h>

class GraphicDevice;

struct PipelineStateDesc final {
	InputLayerReference inputLayer;
	VertexShaderReference vertexShader;
	PixelShaderReference pixelShader;
};

struct PipelineState : public GPUObject {
protected:
	InputLayerReference inputLayer;
	VertexShaderReference vertexShader;
	PixelShaderReference pixelShader;

public:
	PipelineState(GraphicDevice& device, const PipelineStateDesc& desc)
		: GPUObject(device), inputLayer(desc.inputLayer), vertexShader(desc.vertexShader), pixelShader(desc.pixelShader) {}
};

typedef AtomicCounterObjetcBaseReference<PipelineState> PipelineStateReference;