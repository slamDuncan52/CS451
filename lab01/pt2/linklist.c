#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>

/* Our birthday struct */
struct birthday {
        int day;
        int month;
        int year;
        struct list_head list;
};
/* Register birthday_list */
static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int linkedlist_init(void)
{
        int count;
        struct birthday *ptr;
        /* Populate list */
        for(count = 0;count < 5;count++){
                struct birthday *person;
                person = kmalloc(sizeof (*person), GFP_KERNEL);
                person->day = 2 + count;
                person->month = (8 + count)%12;
                person->year = 1995+count;
                INIT_LIST_HEAD(&person->list);
                list_add_tail(&person->list, &birthday_list);
        }
        /* Iterate and Print */
        list_for_each_entry(ptr, &birthday_list, list){
                printk(KERN_INFO "Day: %d Month: %d Year: %d\n",ptr->day,ptr->month,ptr->year);
        }
        return 0;
}

void linkedlist_exit(void) {
        struct birthday *ptr, *next;
        /* Remove all list elems */
        list_for_each_entry_safe(ptr, next, &birthday_list, list){
                list_del(&ptr->list);
                kfree(ptr);
        }
        printk(KERN_INFO "It's gone!\n");
}

/* Macros for registering module entry and exit points. */
module_init( linkedlist_init );
module_exit( linkedlist_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
