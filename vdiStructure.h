#ifndef INTEGRITY_CHECKER_VDISTRUCTURE_H
#define INTEGRITY_CHECKER_VDISTRUCTURE_H

#include <stdint.h>
#include <stdio.h>

typedef struct vdiHeader{
    uint8_t preheader[72];
    uint32_t headerSize;
    uint32_t imageType;
    uint32_t imageFlags;
    uint8_t imageDescription[32];
    uint32_t cylinders;
    uint32_t heads;
    uint32_t sectors;
    uint32_t sectorSize;
    uint32_t offsetBlocks;
    uint32_t offsetData;
    uint32_t unused;
    long long diskSize;
    uint32_t blockSize;
    uint32_t blockExtraData;
    uint32_t blocksInHDD;
    uint32_t blocksAllocated;
    uint8_t UUID[16];
    uint8_t UUIDLastSNAP[16];
    uint8_t UUIDLink[16];
    uint8_t UUIDParent[16];
    uint8_t garbage[56];
} vdiHeader;

typedef struct{
    uint32_t totalInodes;
    uint32_t totalBlocks;
    uint32_t superuserBlocks;
    uint32_t totalUnallocatedBlocks;
    uint32_t totalUnallocatedInodes;
    uint32_t superBlockNumber;
    uint32_t log2BlockSize;
    uint32_t log2FragmentSize;
    uint32_t blocksPerBlockGroup;
    uint32_t fragmentsPerBlockGroup;
    uint32_t inodesPerBlockGroup;
    uint32_t lastMountTime;
    uint32_t lastWriteTime;
    uint16_t numberOfTimesMounted;
    uint16_t numberOfMountsAllowed;
    uint16_t magicNumber;
    uint16_t fileSystemState;
    uint16_t errorHandler;
    uint16_t minorVersion;
    uint32_t PosixLastCheck;
    uint32_t interval;
    uint32_t osID;
    uint32_t majorPortion;
    uint16_t userID;
    uint16_t groupID;
    uint32_t numberOfBlockGroups;
    uint32_t blockSize;
} SuperBlock;

typedef struct{
    uint32_t addressOfBlockUsage;
    uint32_t addressOfInodeUsage;
    uint32_t startingAddressOfInodeTable;
    uint16_t numberOfUnallocatedBlocksInGroup;
    uint16_t numberOfUnallocatedInodesInGroup;
    uint16_t numberOfDirectoriesInGroup;
} BlockGroupDescriptor;

typedef struct{
    uint16_t typeAndPermissions;
    uint16_t userID;
    uint32_t lower32BitsInBytes;
    uint32_t lastAccessTime;
    uint32_t creationTime;
    uint32_t lastModificationTime;
    uint32_t deletionTime;
    uint16_t groupID;
    uint16_t countOfHardLinks;
    uint32_t countOfDiskSectors;
    uint32_t flags;
    uint32_t osSpecificValue1;
    uint32_t directBlockPointer0;
    uint32_t directBlockPointer1;
    uint32_t directBlockPointer2;
    uint32_t directBlockPointer3;
    uint32_t directBlockPointer4;
    uint32_t directBlockPointer5;
    uint32_t directBlockPointer6;
    uint32_t directBlockPointer7;
    uint32_t directBlockPointer8;
    uint32_t directBlockPointer9;
    uint32_t directBlockPointer10;
    uint32_t directBlockPointer11;
    uint32_t singlyIndirectBlockPointer;
    uint32_t doublyIndirectBlockPointer;
    uint32_t triplyIndirectBlockPointer;
    uint32_t generationNumber;
    uint32_t reservedField1;
    uint32_t reservedField2;
    uint32_t blockAddressOfFragment;
    uint32_t osSpecificValue2[12];
} Inode;

typedef struct vdiFile{
    vdiHeader* header;
    long long cursor;
    FILE* file;
    SuperBlock* superBlock;
    BlockGroupDescriptor ** blockGroupDescriptorTable;
}vdiFile;

#endif //INTEGRITY_CHECKER_VDISTRUCTURE_H
