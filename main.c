#include <stdio.h>
#include <inttypes.h>
#include "vdiStructure.h"
#include "vdiFunctions.h"

#define path "../../VDI_Files/Test-fixed-1k.vdi"

int main() {
    vdiFile* vdi = vdiOpen(path);
    printf("Block size in bytes: %d", vdi->superBlock->blockSize);
    int n = 5;
    return 0;
}
