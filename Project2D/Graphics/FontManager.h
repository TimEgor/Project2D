#pragma once

#include <ResourceManager/ResourceName.h>
#include <ResourceManager/ResourceReference.h>

#include <unordered_map>
#include <cstdint>

typedef char CharID;

struct FontCharInfo final {
	uint16_t x, y;
	uint16_t width, height;
	int16_t xoffset, yoffset;
	int16_t xadvance;
};

struct FontInfo final {
	std::unordered_map<CharID, FontCharInfo> chars;
	std::unordered_map<CharID, std::unordered_map<CharID, int16_t>> kernings;
	ResourceReference bitmapTexture;
	uint16_t lineHeight;

	uint16_t getKerning(CharID currentChar, CharID lastChar) const;
};

class FontManager final {
private:
	std::unordered_map<ResourceID, FontInfo> fonts;

	FontManager() = default;

public:
	~FontManager() { release(); }

	static FontManager& get();

	bool init();
	void release();

	bool addFont(const ResourceName& fontName);
	void removeFont(const ResourceName& fontName);
	void removeFont(ResourceID fontID);

	const FontInfo* getFontInfo(const ResourceName& fontName);
	const FontInfo* getFontInfo(ResourceID fontID);
};