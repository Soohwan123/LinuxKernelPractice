#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/stat.h>  

static struct kobject *my_kobj;
static int sysfs_value = 0;

/* `/sys/kernel/mydevice/myattr` 값 읽기 */
static ssize_t myattr_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", sysfs_value);
}

/* `/sys/kernel/mydevice/myattr` 값 쓰기 */
static ssize_t myattr_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%du", &sysfs_value);
    printk(KERN_INFO "mydevice: sysfs_value set to %d\n", sysfs_value);
    return count;
}

/* 🔹 해결 방법: 권한 설정을 `0666` 대신 `S_IRUGO | S_IWUSR`로 변경 */
static struct kobj_attribute myattr = __ATTR(myattr, S_IRUGO | S_IWUSR, myattr_show, myattr_store);

/* 🔹 모듈 로드 시 sysfs 파일 생성 */
static int __init mysysfs_init(void) {
    my_kobj = kobject_create_and_add("mydevice", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;

    if (sysfs_create_file(my_kobj, &myattr.attr)) {
        kobject_put(my_kobj);
        return -EFAULT;
    }

    printk(KERN_INFO "/sys/kernel/mydevice/myattr created\n");
    return 0;
}

/* 🔹 모듈 제거 시 sysfs 파일 삭제 */
static void __exit mysysfs_exit(void) {
    sysfs_remove_file(my_kobj, &myattr.attr);
    kobject_put(my_kobj);
    printk(KERN_INFO "/sys/kernel/mydevice/myattr removed\n");
}

module_init(mysysfs_init);
module_exit(mysysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Soohwan Kim");
MODULE_DESCRIPTION("A simple Sysfs example for kernel 6.x");

