#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/sched/signal.h>
#include <linux/types.h>

static char *process_name = "bash";
module_param(process_name, charp, 0);
MODULE_PARM_DESC(process_name, "Name of the process to find the PID of");

char buffer[256];

int my_strcmp(char *str1, char *str2)
{
    int i;

    i = 0;
    while (str1[i] || str2[i])
    {
        if (str1[i] != str2[i])
            return (str1[i] - str2[i]);
        i++;
    }
    return (0);
}

static int __init pid_module_init(void)
{
    int pids = 1;
    struct task_struct *task_list;
    unsigned int process_count = 0;
    pr_info("%s: In init\n", __func__);
    for_each_process(task_list)
    {
        if (my_strcmp(process_name, task_list->comm) == 0)
        {

            struct mm_struct *mm;
            struct file *exe_file;
            char *result = " ";
            char buf[512];
            mm = task_list->mm;
            exe_file = mm->exe_file;
            if (exe_file)
            {
                get_file(exe_file);
                path_get(&exe_file->f_path);
            }
            if (exe_file)
            {
                result = d_path(&exe_file->f_path, buf, 512);
                path_put(&exe_file->f_path);
            }


            pr_info("-->\n\tname : [%s]\tcommand path: [%s]\t PID:[%d]\t uid: [%d]\t gid: [%d]\t\n",task_list->comm, result, task_list->pid, task_list->cred->uid, pid_vnr(task_pgrp(task_list)));
            process_count++;
        }
    }

    if (process_count > 0)
    {
        pr_info("\n\t-----------------\nfound %d processes with name %s\n\t-----------------\n", process_count, process_name);
    }
    else
    {
        printk(KERN_ERR "Unable to find any process with name '%s'\n", process_name);
    }
    return 0;
}
static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(pid_module_init);
module_exit(hello_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aniket");
