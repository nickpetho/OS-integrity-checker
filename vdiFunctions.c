#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <vdiFunctions.h>

vdiFile* vdiOpen(char *fileName){
    vdiFile* vdi = (vdiFile*) malloc(sizeof(vdiFile));
    vdi ->header = (vdiHeader*) malloc(sizeof(vdiHeader));
    vdi ->superBlock = (SuperBlock*) malloc(sizeof(SuperBlock));
    vdi ->file = fopen(fileName, "rb");
    if(vdi->file == NULL){
        printf("Error: cannot open file");
        vdiClose(vdi);
        return NULL;
    }

    fread(vdi->header->preheader, 1, 72, vdi->file);
    fread(&vdi->header->headerSize, 4, 1, vdi->file);
    fread(&vdi->header->imageType, 4, 1, vdi->file);
    fread(&vdi->header->imageFlags, 4, 1, vdi->file);
    fread(vdi->header->imageDescription, 1, 32, vdi->file);
    fseek(vdi->file, 0x154, 0);
    fread(&vdi->header->offsetBlocks, 4, 1, vdi->file);
    fread(&vdi->header->offsetData, 4, 1, vdi->file);
    fread(&vdi->header->cylinders, 4, 1, vdi->file);
    fread(&vdi->header->heads, 4, 1, vdi->file);
    fread(&vdi->header->sectors, 4, 1, vdi->file);
    fread(&vdi->header->sectorSize, 4, 1, vdi->file);
    fread(&vdi->header->unused, 4, 1, vdi->file);
    fread(&vdi->header->diskSize, 8, 1, vdi->file);
    fread(&vdi->header->blockSize, 4, 1, vdi->file);
    fread(&vdi->header->blockExtraData, 4, 1, vdi->file);
    fread(&vdi->header->blocksInHDD, 4, 1, vdi->file);
    fread(&vdi->header->blocksAllocated, 4, 1, vdi->file);
    fread(vdi->header->UUID, 1, 16, vdi->file);
    fread(vdi->header->UUIDLastSNAP, 1, 16, vdi->file);
    fread(vdi->header->UUIDLink, 1, 16, vdi->file);
    fread(vdi->header->UUIDParent, 1, 16, vdi->file);
    fread(vdi->header->garbage, 1, 56, vdi->file);

    fseek(vdi->file, vdi->header->offsetData + 454, 0);
    uint32_t masterBootRecordStart;
    fread(&masterBootRecordStart, 4, 1, vdi->file);
    masterBootRecordStart = masterBootRecordStart*512;
    vdi->header->offsetData += masterBootRecordStart;

    uint8_t superBlockBuffer[1024];
    vdiSeek(vdi, 1024, 0);
    vdiRead(vdi, superBlockBuffer, 1024);
    fetchBlock(vdi, superBlockBuffer, 0);

    memcpy(&vdi->superBlock->totalInodes, superBlockBuffer, 4);
    memcpy(&vdi->superBlock->totalBlocks, superBlockBuffer + 4, 4);
    memcpy(&vdi->superBlock->superuserBlocks, superBlockBuffer + 8, 4);
    memcpy(&vdi->superBlock->totalUnallocatedBlocks, superBlockBuffer + 12, 4);
    memcpy(&vdi->superBlock->totalUnallocatedInodes, superBlockBuffer + 16, 4);
    memcpy(&vdi->superBlock->superBlockNumber, superBlockBuffer + 20, 4);
    memcpy(&vdi->superBlock->log2BlockSize, superBlockBuffer + 24, 4);
    memcpy(&vdi->superBlock->log2FragmentSize, superBlockBuffer + 28, 4);
    memcpy(&vdi->superBlock->blocksPerBlockGroup, superBlockBuffer + 32, 4);
    memcpy(&vdi->superBlock->fragmentsPerBlockGroup, superBlockBuffer + 36, 4);
    memcpy(&vdi->superBlock->inodesPerBlockGroup, superBlockBuffer + 40, 4);
    memcpy(&vdi->superBlock->lastMountTime, superBlockBuffer + 44, 4);
    memcpy(&vdi->superBlock->lastWriteTime, superBlockBuffer + 48, 4);
    memcpy(&vdi->superBlock->numberOfTimesMounted, superBlockBuffer + 52, 2);
    memcpy(&vdi->superBlock->numberOfMountsAllowed, superBlockBuffer + 54, 2);
    memcpy(&vdi->superBlock->magicNumber, superBlockBuffer + 56, 2);
    memcpy(&vdi->superBlock->fileSystemState, superBlockBuffer + 58, 2);
    memcpy(&vdi->superBlock->errorHandler, superBlockBuffer + 60, 2);
    memcpy(&vdi->superBlock->minorVersion, superBlockBuffer + 62, 2);
    memcpy(&vdi->superBlock->PosixLastCheck, superBlockBuffer + 64, 4);
    memcpy(&vdi->superBlock->interval, superBlockBuffer + 68, 4);
    memcpy(&vdi->superBlock->osID, superBlockBuffer + 72, 4);
    memcpy(&vdi->superBlock->majorPortion, superBlockBuffer + 76, 4);
    memcpy(&vdi->superBlock->userID, superBlockBuffer + 80, 2);
    memcpy(&vdi->superBlock->groupID, superBlockBuffer + 82, 2);

    vdi->superBlock->numberOfBlockGroups = (vdi->superBlock->totalBlocks / vdi->superBlock->blocksPerBlockGroup) + 1;
    vdi->superBlock->blockSize = 1024u << vdi->superBlock->log2BlockSize;

    uint8_t blockGroupTableBuffer[vdi->superBlock->blockSize];
    fetchBlock(vdi, blockGroupTableBuffer, 2);
    vdi->blockGroupDescriptorTable = (BlockGroupDescriptor**) calloc(vdi->superBlock->numberOfBlockGroups, sizeof(BlockGroupDescriptor*));
    int i;
    for(i = 0; i < vdi->superBlock->numberOfBlockGroups; i++){
        vdi->blockGroupDescriptorTable[i] = (BlockGroupDescriptor*) malloc(sizeof(BlockGroupDescriptor));
        memcpy(&vdi->blockGroupDescriptorTable[i]->addressOfBlockUsage, blockGroupTableBuffer + i*32, 4);
        memcpy(&vdi->blockGroupDescriptorTable[i]->addressOfInodeUsage, blockGroupTableBuffer + i*32 + 4 , 4);
        memcpy(&vdi->blockGroupDescriptorTable[i]->startingAddressOfInodeTable, blockGroupTableBuffer + i*32 + 8 , 4);
        memcpy(&vdi->blockGroupDescriptorTable[i]->numberOfUnallocatedBlocksInGroup, blockGroupTableBuffer + i*32 + 12 , 2);
        memcpy(&vdi->blockGroupDescriptorTable[i]->numberOfUnallocatedInodesInGroup, blockGroupTableBuffer + i*32 + 14 , 2);
        memcpy(&vdi->blockGroupDescriptorTable[i]->numberOfDirectoriesInGroup, blockGroupTableBuffer + i*32 + 16 , 2);
    }

    return vdi;
}

void vdiClose(struct vdiFile *vdi){
    if(vdi->blockGroupDescriptorTable != NULL){
        int i;
        for(i = 0; i < vdi->superBlock->numberOfBlockGroups; i++){
            free(vdi->blockGroupDescriptorTable[i]);
        }
        free(vdi->blockGroupDescriptorTable);
    }
    free(vdi->header);
    free(vdi->superBlock);
    fclose(vdi-> file);
    free(vdi);
}

void vdiSeek(vdiFile* vdi, long long offset, int anchor){
    if(anchor == 0){
        vdi->cursor = vdi->header->offsetData + offset;
    }
    if(anchor == 1){
        vdi->cursor += offset;
    }
    if(anchor == 2){
        vdi->cursor = offset + vdi->header->diskSize;
    }
}

void vdiRead(vdiFile* vdi, uint8_t* buffer, size_t nbytes){
    long long vdiBlock = vdi->cursor/vdi->header->blockSize;
    long long offset = vdi->cursor%vdi->header->blockSize;
    long long position = vdiBlock*vdi->header->blockSize+offset;
    fseek(vdi->file, position, 0);
    fread(buffer, 1, nbytes, vdi->file);
}

void fetchBlock(vdiFile* vdi, uint8_t* buffer, uint32_t blockNumber){
    vdiSeek(vdi, blockNumber*vdi->superBlock->blockSize, 0);
    vdiRead(vdi, buffer, vdi->superBlock->blockSize);
}