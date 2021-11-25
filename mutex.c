#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

u64 start = 0;
u64 end = 0;

struct mutex lock;
struct list_head p;

struct my_node {
  struct list_head entry;
  int data;
};

void insert_node(struct list_head* entry, int count) {
  int i = 0;
  
  start = ktime_get_ns();
  for(i=0; i<count; i++) {
    mutex_lock(&lock);
    struct my_node* new;
    new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
    new->data = i;
    list_add_tail(&new->entry, entry);
    mutex_unlock(&lock);
  }
  end = ktime_get_ns();
  
  printk("Mutex linked list insert time: %llu ns\n", end-start);
  
}

void search_node(struct list_head* entry) {
  int count = 0;
  struct list_head* p;
  struct my_node* node;
  
  start = ktime_get_ns();
  
  list_for_each(p, entry) {
    mutex_lock(&lock);
    node = list_entry(p, struct my_node, entry);
    count++;
    mutex_unlock(&lock);
  }
  
  end = ktime_get_ns();
  
  printk("Mutex linked list search time: %llu ns\n", end-start);
}

void delete_node(struct list_head* entry) {
  int count = 0;
  struct list_head* p;
  struct list_head* tmp;
  struct my_node* node;
  
  start = ktime_get_ns();
  
  list_for_each_safe(p, tmp, entry) {
    mutex_lock(&lock);
    node = list_entry(p, struct my_node, entry);
    list_del(p);
    kfree(node);
    count++;
    mutex_unlock(&lock);
  }
  end = ktime_get_ns();
  
  printk("Mutex linked list delete time: %llu ns\n", end-start);
}

int threads(void* data) {
  insert_node(&p, 25000);
  search_node(&p);
  delete_node(&p);
  return 0;
}

int __init linked_list_init(void) {
  INIT_LIST_HEAD(&p);
  mutex_init(&lock);
  int i;
  for(i=0; i<4; i++) {
    kthread_run(threads, NULL, "threads");
  }
  
  return 0;
}

void __exit linked_list_cleanup(void) {
  printk("Bye Module!\n");
}

module_init(linked_list_init);
module_exit(linked_list_cleanup);
MODULE_LICENSE("GPL");
