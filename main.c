#include <stdio.h>
#include <math.h>
#include "vdiStructure.h"
#include "vdiFunctions.h"

#define path "../../VDI_Files/Test-fixed-1k.vdi"

int main() {
    printf("Ext2 Filesystem Integrity Checker\n---------------------------------");
    vdiFile* vdi = vdiOpen(path);
    //printf("Total filesystem size in bytes: %d", );
    //printf("\nSize available for files (used and unused): %d", );
    printf("\nAmount of inodes currently used: %d", (vdi->superBlock->totalInodes)-(vdi->superBlock->totalUnallocatedInodes));
    printf("\nAmount of blocks currently used: %d", (vdi->superBlock->totalBlocks)-(vdi->superBlock->totalUnallocatedBlocks));
    printf("\nNumber of possible files and directories (number of inodes): %d", vdi->superBlock->totalInodes);
    //printf("\nNumber of existing files: %d", );
    //printf("\nNumber of existing directories: %d", );
    printf("\nNumber of block groups: %d", (vdi->superBlock->totalInodes)/(vdi->superBlock->inodesPerBlockGroup));//(vdi->superBlock->totalBlocks)/(vdi->superBlock->blocksPerBlockGroup)+1);
    printf("\n\nBlock Group 1 Info");
    printf("\nBlock address of block usage bitmap: %d", (vdi->blockGroupDescriptorTable->addressOfBlockUsage));
    printf("\nBlock address of inode usage bitmap: %d", (vdi->blockGroupDescriptorTable->addressOfInodeUsage));
    printf("\nStarting block address of inode table: %d", (vdi->blockGroupDescriptorTable->startingAddressOfInodeTable));
    printf("\nNumber of unallocated blocks in group: %d", (vdi->blockGroupDescriptorTable->numberOfUnallocatedBlocksInGroup));
    printf("\nNumber of unallocated inodes in group: %d", (vdi->blockGroupDescriptorTable->numberOfUnallocatedInodesInGroup));
    printf("\nNumber of directories in group: %d", (vdi->blockGroupDescriptorTable->numberOfDirectoriesInGroup));

    /*
     * Write a for loop to iterate through each block group and display their descriptor table
     */

    /* printf("\n\nBlock Group 2 Info");
    printf("\nBlock address of block usage bitmap: %d", (vdi->blockGroupDescriptorTable->addressOfBlockUsage));
    printf("\nBlock address of inode usage bitmap: %d", (vdi->blockGroupDescriptorTable->addressOfInodeUsage));
    printf("\nStarting block address of inode table: %d", (vdi->blockGroupDescriptorTable->startingAddressOfInodeTable));
    printf("\nNumber of unallocated blocks in group: %d", (vdi->blockGroupDescriptorTable->numberOfUnallocatedBlocksInGroup));
    printf("\nNumber of unallocated inodes in group: %d", (vdi->blockGroupDescriptorTable->numberOfUnallocatedInodesInGroup));
    printf("\nNumber of directories in group: %d", (vdi->blockGroupDescriptorTable->numberOfDirectoriesInGroup));
    printf("\n\nBlock Group 2: %d", );
    printf("\n\nBlock Group 3: %d", );
    printf("\n\nBlock Group 4: %d", );
    printf("\n\nBlock Group 5: %d", );
    printf("\n\nBlock Group 6: %d", );
    printf("\n\nBlock Group 7: %d", );
    printf("\n\nBlock Group 8: %d", );
    printf("\n\nBlock Group 9: %d", );
    printf("\n\nBlock Group 10: %d", );
    printf("\n\nBlock Group 11: %d", );
    printf("\n\nBlock Group 12: %d", );
    printf("\n\nBlock Group 13: %d", );
    printf("\n\nBlock Group 14: %d", );
    printf("\n\nBlock Group 15: %d", );
    printf("\n\nBlock Group 16: %d", );*/
    printf("\n\nBlock size in bytes: %d", (1024) - (vdi->superBlock->log2BlockSize));
    printf("\nState of the filesystem: %d",vdi->superBlock->fileSystemState);

    int n = 5;
    vdiClose(vdi);
    return 0;
}
