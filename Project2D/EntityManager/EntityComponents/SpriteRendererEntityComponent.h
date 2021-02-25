#pragma once

#include <EntityManager/EntityComponent.h>
#include <ResourceManager/ResourceReference.h>

#include <atomic>

//
//	CustomProperties
//

class CustomShaderPropertiesReference;

class CustomShaderProperties final {
	friend CustomShaderPropertiesReference;

private:
	std::atomic_size_t counter;

public:
	void* data;

	CustomShaderProperties();
	~CustomShaderProperties();
};

class CustomShaderPropertiesReference final {
private:
	CustomShaderProperties* properties;

public:
	CustomShaderPropertiesReference(CustomShaderProperties* properties);
	CustomShaderPropertiesReference(const CustomShaderPropertiesReference& reference);
	~CustomShaderPropertiesReference();

	bool isNull() const { return properties; }
	void* getPropertiesData(size_t offset = 0) { return (uint8_t*)(properties->data) + offset; }

	void* initNewPropertiesData();
	void releasePropertiesData();
};

class CustomMaterialProperties final {
private:
	CustomShaderPropertiesReference vertexShaderProperties;
	CustomShaderPropertiesReference pixelShaderProperties;

public:
	CustomMaterialProperties() : vertexShaderProperties(nullptr), pixelShaderProperties(nullptr) {}

	bool isNull() const { return vertexShaderProperties.isNull() && pixelShaderProperties.isNull(); }

	CustomShaderPropertiesReference getVertexShaderProperties() { return vertexShaderProperties; }
	CustomShaderPropertiesReference getPixelShaderProperties() { return pixelShaderProperties; }
};

//
//	SpriteRenderer
//

class SpriteRendererEntityComponent final : public EntityComponent {
private:
	ResourceReference sprite;
	ResourceReference material;
	//CustomMaterialProperties customMaterialProperties;

public:
	SpriteRendererEntityComponent();
	SpriteRendererEntityComponent(ResourceReference spriteResource, ResourceReference materialResource = nullptr);

	ResourceReference getSpriteResource() { return sprite; }
	void setSpriteResource(ResourceReference newSpriteResource) { sprite = newSpriteResource; }
	ResourceReference getMaterialResource() { return material; }
	void setMaterialResource(ResourceReference newMaterialResource);

	virtual EntityComponentType getEntityComponentType() const override { return SpriteRendererEntityComponentType; };
};