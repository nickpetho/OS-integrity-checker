#ifndef INTEGRITY_CHECKER_VDIFUNCTIONS_H
#define INTEGRITY_CHECKER_VDIFUNCTIONS_H

#include "vdiStructure.h"

vdiFile* vdiOpen(char *fileName);
void vdiRead(vdiFile* vdi, uint8_t* buffer, size_t nbytes);
void vdiClose(struct vdiFile *vdi);
void vdiSeek(vdiFile* vdi, long long offset, int anchor);
void fetchBlock(vdiFile* vdi, uint8_t* buffer, uint32_t blockNumber);

#endif //INTEGRITY_CHECKER_VDIFUNCTIONS_H
