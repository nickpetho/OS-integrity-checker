#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <vdiFunctions.h>

struct vdiFile *vdiOpen(char *vdiName){
    struct vdiFile *vdiPtr;
    vdiPtr = (struct vdiFile*) malloc(sizeof(struct vdiFile));
    vdiPtr -> fileDes = open(vdiName, 0_RDWR);
    if(vdiPtr ->  fileDes < 0){
        return NULL;
    }

    read(vdiPtr -> fileDes, &vdiPtr -> vdiStruct, sizeof(vdiStructure));
}

void vdiClose(struct vdiFile *vdiPtr){
    close(vdiPtr -> fileDes);
    free(vdiPtr);
}

/*int vdiRead(struct *vdiPtr, int *buffer, int length){
 *
 * }
 *
 * int vdiWrite(struct *vdiPtr, int *buffer, int length){
 *
 * }
 */

int64_t vdiSeek(struct vdiFile *vdiPtr, int offset, int anchorPoint){
    int64_t newCursor;

    if(anchorPoint == 0){ //seek start
        newCursor = offset;
    }else if(anchorPoint == 1){ //seek cursor
        newCursor = vdiPtr -> cursor + offset;
    }else if(anchorPoint == 2){ //seek end
        newCursor = vdiPtr -> vdiStruct.diskSize + offset;
    }else{
        return -1;
    }

    if(newCursor >= 0 || newCursor <= (vdiPtr -> vdiStruct.diskSize)){
        return newCursor;
    }else{
        return -1;
    }
}