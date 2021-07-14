#pragma once

#include <ResourceManager/Resource.h>
#include <Graphics/GraphicDevice.h>

class Texture2DResource final : public Resource {
private:
	Texture2DReference textureReference;

public:
	Texture2DResource(Texture2DReference texture) : textureReference(texture) {}

	Texture2DReference getTextureReference() { return textureReference; }

	virtual ResourceType getResourceType() const override { return Texture2DResourceType; }
};