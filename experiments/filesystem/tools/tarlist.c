#include <stdio.h>
#include "tar.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <tarfile>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	tar_header h = tar_first_header(f);
	while (h.filename[0] != '\0') {
		print_tar_header(&h);
		h = tar_next_header(f, &h);
	}
	fclose(f);
}
