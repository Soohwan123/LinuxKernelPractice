#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_device"
#define MY_IOCTL_CMD _IOW('a', 1, int)

static int my_value =  0;

/* IOCTL 처리함수 */
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    switch (cmd) {
        case MY_IOCTL_CMD:
            copy_from_user(&my_value, (int *)arg, sizeof(int));
            printk(KERN_INFO "ioctl_device: Received value %d\n", my_value);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

/* 파일 연산 구조체 */
static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
};

// 모듈 로드
static int __init myioctl_init(void){
    int major = register_chrdev(0, DEVICE_NAME, &fops);
    if(major < 0){
        printk(KERN_ALERT "Failed to register a major number\n");
        return major;
    }
    printk(KERN_INFO "ioctl_device: registered correctly with major number %d\n", major);
    return 0;
}

// 모듈 언로드
static void __exit myioctl_exit(void){
    unregister_chrdev(0, DEVICE_NAME);
    printk(KERN_INFO "ioctl_device: Goodbye\n");
}

module_init(myioctl_init);
module_exit(myioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Soohwan Kim");
MODULE_DESCRIPTION("A simple ioctl example");
