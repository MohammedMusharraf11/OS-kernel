#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init process_tracker_init(void){
    printk(KERN_INFO "Kernel Module Loaded: Process Tracker\n");
    return 0;
}

static void __exit process_tracker_exit(void){
    printk(KERN_INFO "Kernel Module Unloaded: Process Tracker\n");
}

module_init(process_tracker_init);
module_exit(process_tracker_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Tracker Kernel Module");
MODULE_AUTHOR("Your Name");
