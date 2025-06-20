// xargs for xv6 
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define STDIN 0
#define STDERR 2
#define BUF_SIZE 64

int readline(int fd, void* buf, int size);
int spiltWhitespace(char **src, char *buf);

int
main(int argc, char* argv[]) {
    int n;
    char buf[512];
    while ((n = readline(STDIN, buf, sizeof(buf))) > 0) {
        if (argc < 3) {
            fprintf(STDERR, "Usage: xargs cmd [parms]...\n");
            exit(1);
        }
        char* args[MAXARG];
        int i;
        for (i = 0; i < argc - 1; i++) {
            args[i] = argv[i + 1];
        }

        // printf("buf: [%s], size: [%d], read size: [%d]\n", buf, strlen(buf), n);

        char *tmp = buf;
        char token[512];
        int start_malloc = i;
        while (spiltWhitespace(&tmp, token) > 0) {
            // printf("token: [%s]\t", token);
            // 需要free
            args[i] = malloc(sizeof(token));
            strcpy(args[i], token);
            i++;
            token[0] = '\0';
        }
        // printf("\n");
        
        // for (int l = 0; l < i; l++) {
        //     printf("args[%d]: %s\n", l, args[l]);
        // }

        int forkid = fork();
        if (forkid == 0) {
            int ret = exec(argv[1], args);
            while (args[start_malloc]) {
                // free
                free(args[start_malloc++]);
            }
            exit(ret);
        } else if (forkid < 0) {
            fprintf(STDERR, "xargs: fork failed...\n");
            exit(1);
        }
    }

    while (wait(0) > 0);
    exit(0);
}

int readline(int fd, void* buf, int size) {
    char ch;
    int count = 0;
    while (read(fd, &ch, 1) == 1) {
        if (ch == '\n' || ch == '\0') {
            *((char *)buf) = '\0';
            return count;
        }
        *((char *)buf++) = ch;
        count++;
    }
    return 0;
}

int spiltWhitespace(char **src, char *buf) {
    char *tmp = buf;
    while (**src) {
        if (**src == ' ' || **src == '\t') {
            // 头处遇到
            if (strlen(buf) == 0) {
                (*src)++;
                continue;
            } else {
                *tmp = '\0';
                return strlen(buf);
            }
        } else {
            *(tmp++) = **src;
        }
        (*src)++;
    }
    *tmp = '\0';
    return strlen(buf);
}