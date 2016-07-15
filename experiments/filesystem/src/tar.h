#ifndef TAR_H
#define TAR_H
#include <stdio.h>
typedef struct tar_header {
	char filename[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char typeflag[1];
} tar_header;

tar_header tar_first_header(FILE *);
tar_header tar_next_header(FILE *, tar_header *);
void print_tar_header(tar_header *);
#endif
