#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

int pid = 0; 
module_param(pid, int, 0);
static int __init Hello(void) {
    struct task_struct *task;
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_ERR "Invalid PID\n");
        return -EINVAL;
    }
    printk("PID %d\n", task->pid);
    printk("User ID %d\n", task->cred->uid.val);
    printk("Process Group ID %d\n", task->group_leader->pid);
    printk("Command Path %s\n", task->comm);
    return 0;
}

static void __exit Bye(void) {
    printk("Sayonara");
}

module_init(Hello);
module_exit(Bye);
