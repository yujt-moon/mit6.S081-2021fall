// primes for xv6
#include "kernel/types.h"
#include "user/user.h"

#define NUM 31
#define STDERR 2
#define START_LEVEL 2

void fork_and_pass(int* nums, int index);

int
main(int argc, char* argv[]) {
    int lpipefd[2];
    if (pipe(lpipefd) < 0) {
        fprintf(STDERR, "create pipe failed...\n");
        exit(1);
    }
    fork_and_pass(lpipefd, 0);
}

__attribute__((noreturn)) void fork_and_pass(int* lpipefd, int index) {
    // 退出递归
    if (index >= NUM) {
        exit(0);
    }

    int rpipefd[2];
    if (pipe(rpipefd) < 0) {
        fprintf(STDERR, "create pipe failed...\n");
        exit(1);
    }

    int forkid = fork();
    // 父进程负责向管道写数据
    if (forkid > 0) {
        close(lpipefd[0]);
        close(rpipefd[0]);
        close(rpipefd[1]);
        if (index == 0) {
            for (int i = 2; i <= NUM; i++) {
                write(lpipefd[1], &i, sizeof(int));
            }
        }
        close(lpipefd[1]);
        wait(0);
        exit(0);
    } else if (forkid == 0) {   // 子进程负责消费子进程中的数据
        // 子进程只读不写，所以应该关闭写端：否则，即使父进程已经关闭写端，子进程仍然保留着写端的拷贝，read() 不会返回 EOF。
        close(lpipefd[1]);
        int p;
        while (read(lpipefd[0], &p, sizeof(int)) != 0) {
            if ((index + 2) == p) {
                printf("prime %d\n", p);
            } else if (p % (index + 2) != 0) {
                write(rpipefd[1], &p, sizeof(int));
            }
        }
        close(lpipefd[0]);
        fork_and_pass(rpipefd, index + 1);
        exit(0);
    } else {
        fprintf(STDERR, "fork failed...\n");
        exit(1);
    }
}