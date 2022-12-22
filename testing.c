#include <linux/types.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/cred.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uidgid.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/moduleparam.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paras Dhiman");
MODULE_DESCRIPTION( "The PID of the process whose task_struct details we want to retrieve and print");

static int pid = -1;
struct task_struct *task;
module_param(pid,int ,S_IRUGO);
MODULE_PARM_DESC(pid,"The pid of task command line");

static int __init paras_init(void)
{
       	task = pid_task(find_vpid(pid),PIDTYPE_PID);
	
	printk("Looking up task_struct details for PID %d\n", pid);
	if (task) {
		printk("pid: %d\n",task->pid);
		printk("user_id: %d\n",task->cred->uid);
		printk("pgid: %d\n",current->cred->egid);
		printk("command path: %d\n",task->comm);
	}
	else
	{
		printk("NO VALID FUNCTION\n")
	}

	return 0;
}

static void __exit paras_exit(void) {
	printk("Exiting task_struct_details module\n");
	// No cleanup needed
}

module_init(paras_init);
module_exit(paras_exit);


