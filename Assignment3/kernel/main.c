#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "condvar.h"
#include "defs.h"
#include "barrier.h"
#include "buffer_cond_elem.h"

volatile static int started = 0;

extern int sched_policy;

extern struct barrier barrier_array[BARRIER_ARRAY_SIZE];

extern struct sleeplock lock_delete;
extern struct sleeplock lock_insert;
extern struct sleeplock lock_print;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  sched_policy = SCHED_PREEMPT_RR;

  for (int i = 0; i < BARRIER_ARRAY_SIZE; i++)
  {
    barrier_array[i].n=-1;
    initsleeplock(&(barrier_array[i].lk),"barr lock");
  }
  
  scheduler();        
}
