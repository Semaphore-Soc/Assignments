#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "batchstat.h"

volatile static int started = 0;

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

  currentpolicy=SCHED_PREEMPT_RR;

  currbatch.bsize=0;
  currbatch.stime=__INT32_MAX__;
  currbatch.endtime=0;
  currbatch.exectime=0;
  currbatch.finsihed=0;
  currbatch.turnaroundtime=0;
  currbatch.waitingtime=0;
  currbatch.cumulativectime=0;
  currbatch.maxctime=0;
  currbatch.minctime=__INT32_MAX__;
  currbatch.nbursts=0;
  currbatch.totalblength=0;
  currbatch.maxblength=0;
  currbatch.minblength=__INT32_MAX__;
  currbatch.enbursts=0;
  currbatch.etotalblength=0;
  currbatch.emaxblength=0;
  currbatch.eminblength=__INT32_MAX__;
  currbatch.nerrorblength=0;
  currbatch.eberror=0;

  scheduler();        
}
