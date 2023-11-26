struct barrier
{
    int n;
    struct sleeplock lk;
    cond_t cv;
};
