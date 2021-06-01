#pragma once

#include <EntityManager/EntityComponent.h>
#include <MemoryManager/CustomHeapAllocator.h>
#include <ResourceManager/ResourceReference.h>
#include <ResourceManager/ResourceName.h>
#include <Graphics/Verteces.h>

#include <string>

typedef char LabelTextChar;
typedef std::basic_string<LabelTextChar, std::char_traits<LabelTextChar>, CustomHeapAllocator<LabelTextChar>> LabelText;

struct LabelRect final {
	float x = 0.0f;
	float y = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
};

class CanvasLabelEntityComponent final : public EntityComponent {
protected:
	LabelText text = "";
	ResourceReference materialResource;

	SpriteVertex* verteces = nullptr;
	uint16_t* indeces = nullptr;

	ResourceID fontID = ResourceName("Fonts/Arial").hash();
	LabelRect rect;

	size_t allocatedCharSize = 0;

	uint16_t vertecesCount = 0;
	uint16_t indecesCount = 0;

	bool isDirty = false;

	void release();
	void updateBuffers();

public:
	CanvasLabelEntityComponent();
	CanvasLabelEntityComponent(const char* text);
	~CanvasLabelEntityComponent();

	void setText(const char* newText);
	LabelText& getText() { return text; }
	const LabelText& getText() const { return text; }

	ResourceID getFontID() const { return fontID; }

	ResourceReference getMaterialResource() const { return materialResource; }
	void setMaterialResource(ResourceReference newMaterialResource);

	bool isTextDirty() const { return isDirty; }
	void markDirty() { isDirty = true; }

	LabelRect getLabelRect() const { return rect; }

	void update();

	SpriteVertex* getVerteces() const { return verteces; }
	uint16_t* getIndeces() const { return indeces; }

	uint16_t getVertecesCount() const { return vertecesCount; }
	uint16_t getIndecesCount() const { return indecesCount; }

	virtual EntityComponentType getEntityComponentType() const override { return CanvasLabelEntityComponentType; }
	static EntityComponentType getType() { return CanvasLabelEntityComponentType; }
};