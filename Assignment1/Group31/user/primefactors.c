#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

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
        printf("Error: <n> not an integer. Aborting...\n");
        exit(1);
    }
    n *= neg;
    return n;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Syntax: primefactors <n>. Aborting...\n");
        exit(1);
    }
    int n = atoimod(argv[1]);
    if (n < 2 || n > 100)
    {
        printf("Error: value of <n> out of range. Aborting...\n");
        exit(1);
    }
    int fd[2];
    if (pipe(fd) < 0)
    {
        printf("Error: cannot create pipe. Aborting...\n");
        exit(1);
    }
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int flag = 0;
    while (n > 1 && n % primes[0] == 0)
    {
        flag = 1;
        printf("%d, ", primes[0]);
        n = n / primes[0];
    }
    if (flag == 1)
    {
        printf("[%d]\n", getpid());
        flag = 0;
    }
    if (write(fd[1], &n, sizeof(n)) < 0)
    {
        printf("Error: cannot write. Aborting...\n");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }
    if (n<=1)
    {
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    for (int i = 1; i < 25 && n > 1 ; i++)
    {
        if (fork() == 0)
        {
            if (read(fd[0], &n, sizeof(n)) < 0)
            {
                printf("Error: cannot read. Aborting...\n");
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }

            while (n > 1 && n % primes[i] == 0)
            {
                flag = 1;
                printf("%d, ", primes[i]);
                n = n / primes[i];
            }
            if (flag == 1)
            {
                printf("[%d]\n", getpid());
                flag = 0;
            }
            if (write(fd[1], &n, sizeof(n)) < 0)
            {
                printf("Error: cannot write. Aborting...\n");
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
            if (n == 1)
            {
                break;
            }
            exit(0);
        }
        else
        {
            wait(0);
        }
        if (read(fd[0], &n, sizeof(n)) < 0)
        {
            printf("Error: cannot read. Aborting...\n");
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        if (n <= 1)
        {
            break;
        }
        else
        {
            if (write(fd[1], &n, sizeof(n)) < 0)
            {
                printf("Error: cannot write. Aborting...\n");
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
        }
    }
    close(fd[0]);
    close(fd[1]);
    exit(0);
}