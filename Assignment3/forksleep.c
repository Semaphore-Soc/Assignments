#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int atoimod(const char *s)
{
    int n = 0;
    int neg = 1;
    if (*s == '-')
    {
        neg = -1;
        s++;
    }
    while ('0' <= *s && *s <= '9')
    {
        n = n * 10 + *s++ - '0';
    }
    if (*s != '\0')
    {
        printf("Argument not an integer. Aborting...\n");
        exit(1);
    }
    n *= neg;
    return n;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: forksleep <m> <n>. Aborting...\n");
        exit(1);
    }
    int m = atoimod(argv[1]);
    int n = atoimod(argv[2]);
    if (m <= 0)
    {
        printf("<m> must be greater than 0. Aborting...\n");
        exit(1);
    }
    if (n != 0 && n != 1)
    {
        printf("<n> must be either 0 or 1. Aborting...\n");
        exit(1);
    }
    int f = fork();
    if (f < 0)
    {
        printf("Error in fork. Aborting...\n");
        exit(1);
    }
    if (f == 0)
    {
        if (n == 0)
        {
            sleep(m);
        }
        printf("%d: Child.\n", getpid());
    }
    else
    {
        if (n == 1)
        {
            sleep(m);
        }
        printf("%d: Parent.\n", getpid());
        wait(0);
    }
    exit(0);
}
