#include "vdiFunctions.h"
#include <stdio.h>
#include <inttypes.h>

#define SEEK_START 0
#define SEEK_CURSOR 1
#define SEEK_END 2

int main(int argv, char *argv[]) {
    char *fileName;
    struct vdiFile *vdiPtr;
    fileName = argv[1];
    vdiPtr = vdiOpen(fileName);

}
