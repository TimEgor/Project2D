#include "ZipFile.h"

#include <MemoryManager/MemoryCore.h>

#include <zlib.h>

#include <cassert>

bool ZipFile::init(const ResourceName& archiveName) {
    fopen_s(&zipFile, archiveName.c_str(), "rb");

    if (!zipFile) {
        return false;
    }

    fseek(zipFile, 0L, SEEK_END);
    long currentPos = ftell(zipFile);
    currentPos -= sizeof(ZipDirHeader);

    uint32_t signature = 0;

    bool signatureFound = false;

    while (currentPos >= 0L) {
        fseek(zipFile, currentPos, SEEK_SET);

        fread(&signature, sizeof(uint32_t), 1, zipFile);

        if (signature == ZipDirHeader::SIGNATURE) {
            signatureFound = true;
            break;
        }

        --currentPos;
    }

    assert(signatureFound && "Zip file SIGNATURE haven't been found");

    fseek(zipFile, currentPos, SEEK_SET);

    ZipDirHeader info;
    fread(&info, sizeof(ZipDirHeader), 1, zipFile);

    centralDir = memAllocate(sizeof(ZipDirFileHeader*) * info.nDirEntries + info.dirSize);

    centralDirEntryHeaders = (ZipDirFileHeader**)((uint8_t*)(centralDir) + info.dirSize);

    fseek(zipFile, info.dirOffset, SEEK_SET);
    fread(centralDir, info.dirSize, 1, zipFile);

    bool initEntries = true;
    ZipDirFileHeader* dirEntryHeader = (ZipDirFileHeader*)(centralDir);
    for (size_t i = 0; i < info.nDirEntries; ++i) {
        if (dirEntryHeader->sig != ZipDirFileHeader::SIGNATURE) {
            initEntries = false;
            break;
        }

        if (dirEntryHeader->fnameLen > MAX_RESOURCE_NAME_PATH) {
            initEntries = false;
            break;
        }

        centralDirEntryHeaders[i] = dirEntryHeader;

        char* entryRawName = (char*)(dirEntryHeader) + sizeof(ZipDirFileHeader);

        ResourceName entryName(entryRawName);
        entryName.data()[dirEntryHeader->fnameLen] = '\0';

        files.insert(std::make_pair(entryName.hash(), i));

        dirEntryHeader = (ZipDirFileHeader*)(entryRawName + dirEntryHeader->fnameLen + dirEntryHeader->xtraLen + dirEntryHeader->cmntLen);
    }

    if (!initEntries) {
        release();
        return false;
    }

    return true;
}

void ZipFile::release() {
    if (centralDir) {
        memRelease(centralDir);
        centralDir = nullptr;
    }

    files.clear();

    if (zipFile) {
        fclose(zipFile);
        zipFile = nullptr;
    }
}

bool ZipFile::readFileData(ResourceID fileNameHash, void* data, size_t fileSize) {
    if (!data) {
        return false;
    }

    size_t size = 0;

    auto indexIter = files.find(fileNameHash);
    if (indexIter == files.end()) {
        return false;
    }
    
    fseek(zipFile, centralDirEntryHeaders[indexIter->second]->hdrOffset, SEEK_SET);
    ZipLocalHeader fileHeader;

    fread(&fileHeader, sizeof(ZipLocalHeader), 1, zipFile);
    if (fileHeader.sig != ZipLocalHeader::SIGNATURE)
        return false;

    fseek(zipFile, fileHeader.fnameLen + fileHeader.xtraLen, SEEK_CUR);

    if (fileHeader.compression == Z_NO_COMPRESSION) {
        fread(data, fileHeader.cSize, 1, zipFile);
        return true;
    }
    
    if (fileHeader.compression == Z_DEFLATED) {
        size_t compressedSize = fileHeader.cSize;

        uint8_t* compressedData = (uint8_t*)(memAllocate(compressedSize));
        assert(compressedData);

        fread(compressedData, compressedSize, 1, zipFile);

        z_stream uncompressingStream;
        int uncompressingResult;

        uncompressingStream.next_in = compressedData;
        uncompressingStream.avail_in = compressedSize;
        uncompressingStream.next_out = (uint8_t*)(data);
        uncompressingStream.avail_out = fileSize;
        uncompressingStream.zalloc = (alloc_func)(nullptr);
        uncompressingStream.zfree = (free_func)(nullptr);

        uncompressingResult = inflateInit2(&uncompressingStream, -MAX_WBITS);
        if (uncompressingResult == Z_OK) {
            uncompressingResult = inflate(&uncompressingStream, Z_FINISH);
            inflateEnd(&uncompressingStream);
            if (uncompressingResult == Z_STREAM_END)
                uncompressingResult = Z_OK;
            inflateEnd(&uncompressingStream);
        }
        assert(uncompressingResult == Z_OK);

        memRelease(compressedData);
        return true;
    }

    return false;
}

bool ZipFile::getFileDataSize(ResourceID fileNameHash, size_t& fileSize) {
    auto indexIter = files.find(fileNameHash);
    if (indexIter == files.end()) {
        return false;
    }

    fileSize = centralDirEntryHeaders[indexIter->second]->ucSize;

    return true;
}

ResourceName ZipFile::getFileName(ResourceID fileNameHash) {
    auto indexIter = files.find(fileNameHash);
    if (indexIter == files.end()) {
        return "";
    }

    ZipDirFileHeader* dirEntryHeader = centralDirEntryHeaders[indexIter->second];

    char* entryRawName = (char*)(dirEntryHeader)+sizeof(ZipDirFileHeader);

    ResourceName entryName(entryRawName);
    entryName.data()[dirEntryHeader->fnameLen] = '\0';

    return entryName;
}
