#include <ResourceManager/Resource.h>
#include <Graphics/GraphicDevice.h>

class VertexShaderResource final : public Resource {
private:
	VertexShaderReference shader;

public:
	VertexShaderResource(VertexShaderReference texture) : shader(shader) {}

	VertexShaderReference getShaderReference() { return shader; }

	virtual ResourceType getResourceType() const override { return VertexShaderResourceType; }
};

class PixelShaderResource final : public Resource {
private:
	PixelShaderReference shader;

public:
	PixelShaderResource(PixelShaderReference texture) : shader(shader) {}

	PixelShaderReference getShaderReference() { return shader; }

	virtual ResourceType getResourceType() const override { return PixelShaderResourceType; }
};