#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "myproc"

static char proc_data[100] = "Initial kernel data\n";

/* /proc/myproc 에서 읽기 */
static ssize_t proc_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) { 
	return simple_read_from_buffer(buffer, len, offset, proc_data, strlen(proc_data));
}


/* Procfs 파일에 쓰기 */
static ssize_t proc_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset){
	if(len > sizeof(proc_data) - 1)
		return -EFAULT;
	if(copy_from_user(proc_data, buffer, len))
		return -EFAULT;

	proc_data[len] = '\0'; // 문자열 끝 처리
	printk(KERN_INFO "myproc: Received data :%s\n", proc_data);
	return len;
}

static const struct proc_ops proc_fops = {
	.proc_read = proc_read,
	.proc_write = proc_write,
};



static int __init myproc_init(void) {
	proc_create(PROC_NAME, 0666, NULL, &proc_fops);
	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	return 0;
}


static void __exit myproc_exit(void) {
	remove_proc_entry(PROC_NAME, NULL);
	printk(KERN_INFO, "/proc/%s removed\n", PROC_NAME);
}

module_init(myproc_init);
module_exit(myproc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Soohwan Kim");
MODULE_DESCRIPTION("A simple Procfs example");
