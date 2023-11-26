struct semaphore
{
    int value;
    cond_t cv;
    struct sleeplock lk;
};
