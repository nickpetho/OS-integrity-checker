#include <stdio.h>
#include "vdiStructure.h"
#include "vdiFunctions.h"

int main(int argc, char *argv[]){
    char *fileName;
    fileName = argv[1];

    vdiFile* vdi = vdiOpen(fileName);

    printf("Ext2 Filesystem Integrity Checker\n---------------------------------");
    printf("\nTotal filesystem size in bytes: %d", (vdi->superBlock->totalBlocks)*(vdi->superBlock->blockSize));
    printf("\nSize available for files (used): %d", ((vdi->superBlock->totalBlocks)-(vdi->superBlock->totalUnallocatedBlocks))*(vdi->superBlock->blockSize));
    printf("\nSize available for files (unused): %d",(vdi->superBlock->totalUnallocatedBlocks)*(vdi->superBlock->blockSize));
    printf("\nAmount of space currently used:");
    printf("\n    Amount of inodes currently used: %d", (vdi->superBlock->totalInodes)-(vdi->superBlock->totalUnallocatedInodes));
    printf("\n    Amount of blocks currently used: %d", (vdi->superBlock->totalBlocks)-(vdi->superBlock->totalUnallocatedBlocks));
    printf("\nNumber of possible files and directories (number of inodes): %d", vdi->superBlock->totalInodes);
    printf("\nNumber of block groups: %d", (vdi->superBlock->totalInodes)/(vdi->superBlock->inodesPerBlockGroup));
    int i;
    int numberOfDirectories = 0;
    for(i = 0; i < vdi->superBlock->numberOfBlockGroups; i++){
        numberOfDirectories += vdi->blockGroupDescriptorTable[i]->numberOfDirectoriesInGroup;
        printf("\n\nBlock Group %d Info", i);
        printf("\nBlock address of block usage bitmap: %d", (vdi->blockGroupDescriptorTable[i]->addressOfBlockUsage));
        printf("\nBlock address of inode usage bitmap: %d", (vdi->blockGroupDescriptorTable[i]->addressOfInodeUsage));
        printf("\nStarting block address of inode table: %d", (vdi->blockGroupDescriptorTable[i]->startingAddressOfInodeTable));
        printf("\nNumber of unallocated blocks in group: %d", (vdi->blockGroupDescriptorTable[i]->numberOfUnallocatedBlocksInGroup));
        printf("\nNumber of unallocated inodes in group: %d", (vdi->blockGroupDescriptorTable[i]->numberOfUnallocatedInodesInGroup));
        printf("\nNumber of directories in group: %d", (vdi->blockGroupDescriptorTable[i]->numberOfDirectoriesInGroup));
    }
    printf("\n\nNumber of existing files: %d",((vdi->superBlock->totalInodes)-(vdi->superBlock->totalUnallocatedInodes) - numberOfDirectories));
    printf("\nNumber of directories: %d", numberOfDirectories);
    printf("\nBlock size in bytes: %d", (vdi->superBlock->blockSize));
    printf("\nState of the filesystem: %d",vdi->superBlock->fileSystemState);

    vdiClose(vdi);
    return 0;
}