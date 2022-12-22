#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv Sood");

int pid = 0; 
module_param(pid, int, S_IRUGO|S_IWUSR);

static int __init Iniitializing(void) {
    struct task_struct *task;
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_ERR "Invalid PID\n");
        return -EINVAL;
    }
    else{
        printk("PID %d\n", task->pid);
        printk("User ID %d\n", task->cred->uid.val);
        printk("PGID %d\n", task->group_leader->pid);
        printk("Command Path %s\n", task->comm);
    }
    return 0;
}

static void __exit Exiting(void) {
    printk("Exiting");
}

module_init(Iniitializing);
module_exit(Exiting);
