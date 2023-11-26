#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "condvar.h"
#include "semaphore.h"

void sem_init(struct semaphore *s, int x)
{
    s->value = x;
    // cond_init(&s->cv);
    initsleeplock(&s->lk, "sem lock");
}

void sem_wait(struct semaphore *s)
{
    acquiresleep(&s->lk);
    while (s->value == 0)
        cond_wait(&s->cv, &s->lk);
    s->value--;
    releasesleep(&s->lk);
}

void sem_post(struct semaphore *s)
{
    acquiresleep(&s->lk);
    s->value++;
    cond_signal(&s->cv);
    releasesleep(&s->lk);
}