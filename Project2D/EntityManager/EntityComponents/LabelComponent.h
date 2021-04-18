#pragma once

#include <EntityManager/EntityComponent.h>
#include <MemoryManager/CustomHeapAllocator.h>
#include <ResourceManager/ResourceReference.h>
#include <ResourceManager/ResourceName.h>

#include <string>

typedef char LabelTextChar;
typedef std::basic_string<LabelTextChar, std::char_traits<LabelTextChar>, CustomHeapAllocator<LabelTextChar>> LabelText;

struct LabelRect final {
	float x = 0.0f;
	float y = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
};

class LabelComponent : public EntityComponent {
protected:
	LabelText text = "";
	ResourceReference materialResource;
	ResourceID fontID = ResourceName("Fonts/Arial").hash();
	LabelRect rect;
	bool isDirty = false;

	virtual void updateBuffers() = 0;

public:
	LabelComponent();
	LabelComponent(Heap* heap);
	LabelComponent(const char* text);
	LabelComponent(const char* text, Heap* heap);

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

	virtual EntityComponentType getEntityComponentType() const override { return LabelEntityComponentType; }
};