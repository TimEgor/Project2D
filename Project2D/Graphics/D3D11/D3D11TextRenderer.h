#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <ResourceManager/ResourceName.h>
#include <ResourceManager/ResourceReference.h>

#include <unordered_map>

class D3D11TextRenderer final {
	typedef uint16_t CharID;

	struct FontCharInfo final {
		uint16_t x, y;
		uint16_t u, v;
		CharID id;
	};

	struct FontInfo final {
		std::unordered_map<CharID, FontCharInfo> chars;
		ResourceReference bitmapTexture;
		uint16_t textureWidth, textureHeight;
		uint16_t lineHeight;
	};

private:
	std::unordered_map<ResourceID, FontInfo> fonts;

	D3D11TextRenderer() = default;

public:
	~D3D11TextRenderer() { release(); }

	static D3D11TextRenderer& get();

	bool init();
	void release();

	bool addFont(const ResourceName& fontName);
	void removeFont(const ResourceName& fontName);

	void drawText(size_t strLen, const char* str);
};