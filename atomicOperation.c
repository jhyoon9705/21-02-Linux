#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/init.h>

struct task_struct* kthreads[4];

int i = 0;
int lock = 0;

int fetchAndAdd(void* data) {
  while (!kthread_should_stop()) {
    __sync_fetch_and_add(&i, 1);
    printk("%d, %d, %s\n", current->pid, i, current->comm);
    msleep(500);
    
  }
  return 0;
}

int testAndSet(void* data) {
  while (!kthread_should_stop()) {
    while (__sync_lock_test_and_set(&lock, 1));
    i++;
    printk("%d, %d, %s\n", current->pid, i, current->comm);
    __sync_lock_release(&lock);
    msleep(500);
  }
  return 0;
}

int compareAndSwap(void* data) {
  while (!kthread_should_stop()) {
    while (__sync_val_compare_and_swap(&lock, 0, 1));
    i++;
    printk("%d, %d, %s\n", current->pid, i, current->comm);
    lock = 0;
    msleep(500);
  }
  return 0;
}

int __init my_module_init(void) {
  printk("Start Module!\n");
  kthreads[0] = kthread_run(fetchAndAdd, NULL, "fetchAndAdd");
  kthreads[1] = kthread_run(testAndSet, NULL, "testAndSet");
  kthreads[2] = kthread_run(compareAndSwap, NULL, "compareAndSwap");
  kthreads[3] = kthread_run(fetchAndAdd, NULL, "fetchAndAdd");

  return 0;
}

void __exit my_module_cleanup(void) {
  int i;
  for (i = 0; i < 4; ++i)
    kthread_stop(kthreads[i]);
  printk("Finish Module!\n");
}

module_init(my_module_init);
module_exit(my_module_cleanup);
MODULE_LICENSE("GPL");

