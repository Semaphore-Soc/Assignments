struct buffer_cond_elem
{
   int x;
   int full;
   struct sleeplock lk;
   cond_t inserted;
   cond_t deleted;
};