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
        printf("Error: argument not an integer. Aborting...\n");
        exit(1);
    }
    n *= neg;
    return n;
}

void pipepid(int n, int fd[2])
{
    if (n == 0)
    {
        return;
    }
    int b = fork();
    if (b < 0)
    {
        printf("Error: cannot fork. Aborting...\n");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }
    else if (b == 0)
    {
        int x;
        if (read(fd[0], &x, sizeof(x)) < 0)
        {
            printf("Error: cannot read. Aborting...\n");
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        x = x + getpid();
        printf("%d: %d\n", getpid(), x);
        if (write(fd[1], &x, sizeof(x)) < 0)
        {
            printf("Error: cannot write. Aborting...\n");
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        pipepid(n - 1, fd);
        return;
    }
    else
    {
        wait(0);
        return;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Syntax: pipeline <n> <x>. Aborting...\n");
        exit(1);
    }
    int n = atoimod(argv[1]);
    int x = atoimod(argv[2]);
    if (n <= 0)
    {
        printf("Error: value of <n> must be positive. Aborting...\n");
        exit(1);
    }
    int fd[2];
    if (pipe(fd) < 0)
    {
        printf("Error: cannot create pipe. Aborting...\n");
        exit(1);
    }
    if (write(fd[1], &x, sizeof(x)) < 0)
    {
        printf("Error: cannot write. Aborting...\n");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }
    pipepid(n, fd);
    close(fd[0]);
    close(fd[1]);
    exit(0);
}