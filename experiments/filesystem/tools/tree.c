// Print a directory tree for the given directory, using C standard library
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void visit(char *base, char *path, int indent) {
	for (int i = 0; i < indent; i++) putchar('\t');
	printf("%s", path);

	char *fullpath = malloc(strlen(base)+strlen(path)+2);
	strcpy(fullpath, base);
	if (strcmp("", base) != 0) strcat(fullpath, "/");
	strcat(fullpath, path);

	struct stat statbuf;
	if (stat(fullpath, &statbuf) != 0) {
		putchar('\n');
		perror(fullpath);
		free(fullpath);
		return;
	}
	if (S_ISDIR(statbuf.st_mode)) {
		puts("/");
	} else {
		puts("");
		free(fullpath);
		return;
	}

	DIR *dirp = opendir(fullpath);
	if (!dirp) {
		perror(fullpath);
		free(fullpath);
		return;
	}
	
	struct dirent *ent;
	errno = 0;
	while ((ent = readdir(dirp)) != NULL) {
		if (strcmp(ent->d_name, ".") == 0) continue;
		if (strcmp(ent->d_name, "..") == 0) continue;
		visit(fullpath, ent->d_name, indent+1);
	}
	if (errno) {
		perror(fullpath);
	}

	closedir(dirp);
	free(fullpath);
}

int main(int argc, char **argv) {
	char *path = ".";
	if (argc > 2) {
		fprintf(stderr, "Usage: %s [path]\n", argv[0]);
		return 1;
	} else if (argc == 2) {
		path = argv[1];
	}

	// check if path is a directory
	struct stat statbuf;
	if (stat(path, &statbuf) != 0) {
		perror(path);
		return -errno;
	}
	if (!S_ISDIR(statbuf.st_mode)) {
		fprintf(stderr, "%s is not a directory.\n", path);
		return 1;
	}

	visit("", path, 0);
	return 0;
}
