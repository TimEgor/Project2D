#include "FontManager.h"

#include <ResourceManager/ResourceManager.h>

#pragma pack(push, 1)
struct InfoBlock {
    int32_t blockSize;
    int16_t fontSize;
    int8_t smooth : 1;
    int8_t unicode : 1;
    int8_t italic : 1;
    int8_t bold : 1;
    int8_t reserved : 4;
    uint8_t charSet;
    uint16_t stretchH;
    int8_t aa;
    uint8_t paddingUp;
    uint8_t paddingRight;
    uint8_t paddingDown;
    uint8_t paddingLeft;
    uint8_t spacingHoriz;
    uint8_t spacingVert;
    uint8_t outline;
};

struct CommonBlock {
    int32_t blockSize;
    uint16_t lineHeight;
    uint16_t base;
    uint16_t scaleW;
    uint16_t scaleH;
    uint16_t pages;
    uint8_t reserved : 7;
    uint8_t packed : 1;
    uint8_t alphaChnl;
    uint8_t redChnl;
    uint8_t greenChnl;
    uint8_t blueChnl;
};

struct CharBlock {
    uint32_t id;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t xoffset;
    int16_t yoffset;
    int16_t xadvance;
    int8_t page;
    int8_t channel;
};

struct KerningPairsBlock {
    uint32_t first;
    uint32_t second;
    int16_t amount;
};
#pragma pack(pop)

FontManager& FontManager::get() {
	static FontManager uniqueFontManager;
	return uniqueFontManager;
}

bool FontManager::init() {
	return true;
}

void FontManager::release() {
}

bool FontManager::addFont(const ResourceName& fontName) {
	ResourceID fontID = fontName.hash();

	ResourceManager& resourceManager = ResourceManager::get();

	size_t fontInfoFileSize = 0;
	void* fontInfoData = resourceManager.getRawResourceFromArchive(ResourceName(fontName) + "_info", fontInfoFileSize);
	if (fontInfoData) {
        uint8_t* infoDataPtr = (uint8_t*)(fontInfoData);
        size_t offset = 0;

        if (strncmp((char*)infoDataPtr, "BMF", 3) != 0) {
            delete[] fontInfoData;
            return false;
        }

        FontInfo fontInfo;

        infoDataPtr += 5;

        //InfoBlock
        offset = *((uint32_t*)(infoDataPtr)); 
        infoDataPtr += offset + sizeof(InfoBlock::blockSize); //skiping InfoBlock and fontName

        infoDataPtr += 1;

        //CommonBlock
        fontInfo.lineHeight = *(uint16_t*)(infoDataPtr + sizeof(CommonBlock::blockSize));
        infoDataPtr += sizeof(CommonBlock); //skiping CommonBlock

        infoDataPtr += 1;

        //Pages
        offset = *((uint32_t*)(infoDataPtr));
        infoDataPtr += offset; //skiping Pages

        infoDataPtr += 5;

        //Chars
        offset = *((uint32_t*)(infoDataPtr));
        size_t charsNum = offset / sizeof(CharBlock);

        infoDataPtr += 4;

        fontInfo.chars.reserve(charsNum);

        for (size_t i = 0; i < charsNum; ++i) {
            CharBlock* charBlock = (CharBlock*)(infoDataPtr);

            FontCharInfo charInfo;
            charInfo.x = charBlock->x;
            charInfo.y = charBlock->y;
            charInfo.width = charBlock->width;
            charInfo.height = charBlock->height;
            charInfo.xoffset = charBlock->xoffset;
            charInfo.yoffset = charBlock->yoffset;
            charInfo.xadvance = charBlock->xadvance;

            fontInfo.chars.insert(std::make_pair(charBlock->id, charInfo));
            infoDataPtr += sizeof(CharBlock);
        }

        infoDataPtr += 1;

        //Kernings
        offset = *((uint32_t*)(infoDataPtr));
        size_t kerningsNum = offset / sizeof(KerningPairsBlock);

        infoDataPtr += 4;

        for (size_t i = 0; i < kerningsNum; ++i) {
            KerningPairsBlock* kerningBlock = (KerningPairsBlock*)(infoDataPtr);

            fontInfo.kernings[kerningBlock->first][kerningBlock->second] = kerningBlock->amount;
            infoDataPtr += sizeof(KerningPairsBlock);
        }

        fontInfo.bitmapTexture = resourceManager.getResourceFromArchive(ResourceName(fontName) + ".png");

        fonts.insert(std::make_pair(fontID, fontInfo));

		delete[] fontInfoData;

        return true;
	}

	return false;
}

void FontManager::removeFont(const ResourceName& fontName) {
    removeFont(fontName.hash());
}

void FontManager::removeFont(ResourceID fontID) {
    auto findFontInfoIter = fonts.find(fontID);
    if (findFontInfoIter != fonts.end()) {
        fonts.erase(findFontInfoIter);
    }
}

const FontInfo* FontManager::getFontInfo(const ResourceName& fontName) {
	return getFontInfo(fontName.hash());
}

const FontInfo* FontManager::getFontInfo(ResourceID fontID) {
	auto findFontInfoIter = fonts.find(fontID);
	if (findFontInfoIter != fonts.end()) {
		return &findFontInfoIter->second;
	}

	return nullptr;
}

uint16_t FontInfo::getKerning(CharID currentChar, CharID lastChar) const {
    auto currentCharIter = kernings.find(currentChar);
    if (currentCharIter != kernings.end()) {
        auto lastCharIter = currentCharIter->second.find(currentChar);
        if (lastCharIter != currentCharIter->second.end()) {
            return lastCharIter->second;
        }
    }

    return 0;
}
