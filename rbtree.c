#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>

#define FALSE 0
#define TRUE 1

u64 start = 0;
u64 end = 0;

int rb_insert(struct rb_root* root, int count);
void rb_search(struct rb_root* root, int count);
void rb_delete(struct rb_root* root, int count);

struct my_type {
  struct rb_node node;
  int key;
  int value;
};

int __init rbtree_init(void) {
  struct rb_root my_tree = RB_ROOT;
  
  rb_insert(&my_tree, 1000);
  rb_search(&my_tree, 1000);
  rb_delete(&my_tree, 1000);
  
  rb_insert(&my_tree, 10000);
  rb_search(&my_tree, 10000);
  rb_delete(&my_tree, 10000);
  
  rb_insert(&my_tree, 100000);
  rb_search(&my_tree, 100000);
  rb_delete(&my_tree, 100000);
  
  return 0;
}

void __exit rbtree_cleanup(void) {
  printk("Bye Module!\n");
}

int rb_insert(struct rb_root* root, int count) {
  struct rb_node** new = &(root->rb_node);
  struct rb_node* parent = NULL;
  
  int i = 0;
  
  start = ktime_get_ns();
  for(i=0; i<count; i++) {
   struct my_type* this = kmalloc(sizeof(struct my_type), GFP_KERNEL);
   this->key = i;
   this->value = i;
   
     while(*new) {
       parent = *new;
       if(this->key<rb_entry(parent, struct my_type, node)->key)
         new = &((*new)->rb_left);
       else if(this->key>rb_entry(parent, struct my_type, node)->key)
         new = &((*new)->rb_right);
       else 
         return FALSE;
     }
     rb_link_node(&this->node, parent, new);
     rb_insert_color(&this->node, root);
     
     
 }
  end = ktime_get_ns();
  printk("Inserting %d nodes time: %llu\n", count, end-start);
  return TRUE;
}

void rb_search(struct rb_root* root, int count) {
  struct rb_node* node;
  struct my_type* data;
  
  start = ktime_get_ns();
  for(node = rb_first(root); node; node = rb_next(node)) {
    data = rb_entry(node, struct my_type, node);
  }
  end = ktime_get_ns();
  printk("Searching %d nodes time: %llu\n", count, end-start);
}

void rb_delete(struct rb_root* tree, int count) {
  struct my_type* data;
  struct rb_node* node;
  
  start = ktime_get_ns();
  for(node = rb_first(root); node; node=rb_next(node)) {
    data = rb_entry(node, struct my_type, node);
    rb_erase(&data->node, root);
    kfree(data);
  }
  end = ktime_get_ns();
  printk("Deleting %d nodes time: %llu\n", count, end-start);
}

module_init(rbtree_init);
module_exit(rbtree_cleanup);
MODULE_LICENSE("GPL");
