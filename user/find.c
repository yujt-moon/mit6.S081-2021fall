// find for xv6
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

#define STDERR 2

void recursion_find(char* path, char* filename);

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(STDERR, "Usage: find [/path] [filename]\n");
        exit(1);
    }

    recursion_find(argv[1], argv[2]);

    exit(0);
}

void recursion_find(char* path, char* filename) {
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    if ((fd = open(path, 0)) < 0) {
        fprintf(STDERR, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(STDERR, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_DIR) {
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            p = buf;
            strcpy(p, path);
            p += strlen(path);
            if ('/' != path[strlen(path) - 1]) {
                strcpy(p, "/");
                p += 1;
            }
            strcpy(p, de.name);
            if (strcmp(filename, de.name) == 0) {
                printf("%s\n", buf);
                continue;
            }
            recursion_find(buf, filename);
        }
    }

    close(fd);
    return;
}