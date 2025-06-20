// pingpong for xv6
#include "kernel/types.h"
#include "user/user.h"

#define STDOUT 1
#define STDERR 2
#define BUF_SIZE 64

int
main(int argc, char* argv[])
{
    int pipefd_p[2];
    int pipefd_c[2];
    if (pipe(pipefd_p) < 0) {
        fprintf(STDERR, "create pipe failed...\n");
        exit(1);
    }
    if (pipe(pipefd_c) < 0) {
        fprintf(STDERR, "create pipe failed...\n");
        exit(1);
    }
    int forkid = fork();
    if (forkid == 0) {
        int pid = getpid();
        void* p = malloc(BUF_SIZE);
        read(pipefd_p[0], p, BUF_SIZE);
        fprintf(STDOUT, "%d: received ping\n", pid);
        write(pipefd_c[1], "pong", BUF_SIZE);
        free(p);
    } else if (forkid > 0) {
        int pid = getpid();
        void* p = malloc(BUF_SIZE);
        write(pipefd_p[1], "ping", BUF_SIZE);
        read(pipefd_c[0], p, BUF_SIZE);
        fprintf(STDOUT, "%d: received pong\n", pid);
        free(p);
    } else {
        fprintf(STDERR, "fork failed...\n");
        exit(1);
    }
    close(pipefd_p[0]);
    close(pipefd_p[1]);
    close(pipefd_c[0]);
    close(pipefd_c[1]);
    exit(0);
}