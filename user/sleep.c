// sleep implementation for rsicv64
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(2, "usage: sleep num ticks\n");
        exit(1);
    }
    int tick_num = atoi(argv[1]);
    int status = sleep(tick_num);
    exit(status);
}