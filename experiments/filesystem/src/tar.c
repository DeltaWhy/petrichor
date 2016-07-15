#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "tar.h"

tar_header tar_first_header(FILE *f) {
	tar_header h;
	size_t n = fread(&h, 1, sizeof(tar_header), f);
	if (n != sizeof(tar_header)) {
		if (errno) {
			perror("tar_first_header");
		} else {
			fprintf(stderr, "Bad tar file\n");
		}
	}
	return h;
}

static uint32_t octal_to_u32(char *p) {
	uint32_t res = 0;
	while (*p >= '0' && *p <= '7') {
		res = res << 3;
		res |= (*p - '0');
		p++;
	}
	return res;
}

void print_tar_header(tar_header *p) {
	uint32_t mode = octal_to_u32(p->mode);
	uint32_t uid = octal_to_u32(p->uid);
	uint32_t gid = octal_to_u32(p->gid);
	uint32_t size = octal_to_u32(p->size);
	uint32_t chksum = octal_to_u32(p->chksum);
	printf("Tar header:\n"
			"filename: %s\n"
			"mode: 0%o\n"
			"uid: %d\n"
			"gid: %d\n"
			"size: %d\n"
			"mtime: %s\n"
			"chksum: %d\n"
			"typeflag: %c\n",
			p->filename, mode, uid, gid, size, p->mtime,
			chksum, p->typeflag[0]);
}

tar_header tar_next_header(FILE *f, tar_header *p) {
	tar_header h;
	uint32_t size = (octal_to_u32(p->size) + 511) & ~0x1ff;
	if (fseek(f, 512L - sizeof(tar_header), SEEK_CUR) != 0) {
		perror("tar_next_header");
		return h;
	}
	if (fseek(f, size, SEEK_CUR) != 0) {
		perror("tar_next_header");
		return h;
	}

	size_t n = fread(&h, 1, sizeof(tar_header), f);
	if (n != sizeof(tar_header)) {
		if (errno) {
			perror("tar_next_header");
		} else {
			fprintf(stderr, "Bad tar file\n");
		}
	}
	return h;
}
