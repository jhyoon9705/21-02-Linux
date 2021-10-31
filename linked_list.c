#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>

u64 start = 0;
u64 end = 0;

struct my_node {
  struct list_head entry;
  int data;
};

void insert_node(struct list_head* entry, int count) {
  int i = 0;
  
  start = ktime_get_ns();
  for(i=0; i<count; i++) {
    struct my_node* new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
    new->data = i;
    list_add_tail(&new->entry, entry);
  }
  end = ktime_get_ns();
  
  printk("Inserting %d nodes time: %llu\n", count, end-start);
  
}

void search_node(struct list_head* entry) {
  int count = 0;
  struct list_head* p;
  struct my_node* node;
  
  start = ktime_get_ns();
  list_for_each(p, entry) {
    node = list_entry(p, struct my_node, entry);
    count++;
  }
  end = ktime_get_ns();
  
  printk("Searching %d nodes time: %llu\n", count, end-start);
}

void delete_node(struct list_head* entry) {
  int count = 0;
  struct list_head* p;
  struct list_head* tmp;
  struct my_node* node;
  
  start = ktime_get_ns();
  list_for_each_safe(p, tmp, entry) {
    node = list_entry(p, struct my_node, entry);
    list_del(p);
    kfree(node);
    count++;
  }
  end = ktime_get_ns();
  
  printk("Deleting %d nodes time: %llu\n", count, end-start);
}

int __init linked_list_init(void) {
  struct list_head list;
  INIT_LIST_HEAD(&list);
  
  insert_node(&list, 1000);
  search_node(&list);
  delete_node(&list);
  
  insert_node(&list, 10000);
  search_node(&list);
  delete_node(&list);
  
  insert_node(&list, 100000);
  search_node(&list);
  delete_node(&list);
  
  return 0;
}

void __exit linked_list_cleanup(void) {
  printk("Bye Module!\n");
}

module_init(linked_list_init);
module_exit(linked_list_cleanup);
MODULE_LICENSE("GPL");
