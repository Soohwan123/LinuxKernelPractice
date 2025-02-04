#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydevice"

static int major_number;

/* 디바이스 파일 open */
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydevice: Device opened\n");
    return 0;
}

/* 디바이스 파일 close */
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydevice: Device closed\n");
    return 0;
}

/* 디바이스에서 읽기 */
static ssize_t device_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    char *data = "Hello from kernel!\n";
    size_t data_len = strlen(data);

    if (*offset >= data_len)
        return 0;

    if (len > data_len - *offset)
        len = data_len - *offset;

    if (copy_to_user(buffer, data + *offset, len))
        return -EFAULT;

    *offset += len;
    return len;
}

/* 디바이스에 쓰기 */
static ssize_t device_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
    char user_data[128];

    if (len > sizeof(user_data) - 1)
        len = sizeof(user_data) - 1;

    if (copy_from_user(user_data, buffer, len))
        return -EFAULT;

    user_data[len] = '\0';
    printk(KERN_INFO "mydevice: Received from user: %s\n", user_data);

    return len;
}

/* 파일 연산 구조체 */
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};

/* 모듈 로드 */
static int __init mydevice_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "mydevice: Registered with major number %d\n", major_number);
    return 0;
}

/* 모듈 제거 */
static void __exit mydevice_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "mydevice: Unregistered\n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Soohwan Kim");
MODULE_DESCRIPTION("A simple character device driver example");

