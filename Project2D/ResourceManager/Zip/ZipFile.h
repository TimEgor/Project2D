#pragma once

#include <ResourceManager/Resource.h>
#include <ResourceManager/ResourceName.h>

#include <cstdio>
#include <cstdint>
#include <unordered_map>

#pragma pack(1)

struct ZipLocalHeader {
	static const uint32_t SIGNATURE = 0x04034b50;

	uint32_t sig;
	uint16_t version;
	uint16_t flag;
	uint16_t compression;
	uint16_t modTime;
	uint16_t modDate;
	uint32_t crc32;
	uint32_t cSize;
	uint32_t ucSize;
	uint16_t fnameLen;
	uint16_t xtraLen;
};

struct ZipDirHeader {
	static const uint32_t SIGNATURE = 0x06054b50;

	uint32_t sig;
	uint16_t nDisk;
	uint16_t nStartDisk;
	uint16_t nDirEntries;
	uint16_t totalDirEntries;
	uint32_t dirSize;
	uint32_t dirOffset;
	uint16_t cmntLen;
};

struct ZipDirFileHeader {
	static const uint32_t SIGNATURE = 0x02014b50;

	uint32_t sig;
	uint16_t verMade;
	uint16_t verNeeded;
	uint16_t flag;
	uint16_t compression;
	uint16_t modTime;
	uint16_t modDate;
	uint32_t crc32;
	uint32_t cSize;
	uint32_t ucSize;
	uint16_t fnameLen;
	uint16_t xtraLen;
	uint16_t cmntLen;
	uint16_t diskStart;
	uint16_t intAttr;
	uint32_t extAttr;
	uint32_t hdrOffset;

	char* GetName() const { return (char*)(this + 1); }
	char* GetExtra() const { return GetName() + fnameLen; }
	char* GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()

class ZipFile final {
private:
	std::unordered_map<ResourceID, uint32_t> files;
	void* centralDir;
	ZipDirFileHeader** centralDirEntryHeaders;

	FILE* zipFile;

public:
	ZipFile() : centralDir(nullptr), centralDirEntryHeaders(nullptr), zipFile(nullptr) {}
	~ZipFile() { release(); }

	bool init(const ResourceName &archiveName);
	void release();

	bool readFileData(ResourceID fileNameHash, void* data, size_t fileSize);
	bool getFileDataSize(ResourceID fileNameHash, size_t &fileSize);

	ResourceName getFileName(ResourceID fileNameHash);
};