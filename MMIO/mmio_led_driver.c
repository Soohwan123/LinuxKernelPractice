#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>        // ioremap(), iowrite32(), ioread32()
#include <linux/uaccess.h>   // copy_to_user(), copy_from_user()
#include <linux/cdev.h>      // 캐릭터 디바이스 관련
#include <linux/device.h>    // 디바이스 클래스
                             //

#define DEVICE_NAME "mmio_led"
#define CLASS_NAME "mmio_class"

#define LED_REG_BASE 0x10000000 // 가상의 led 레지스터의 물리 주소
#define LED_REG_SIZE 4          // 4바이트 크기의 레지스터

static int majorNumber;
static struct class* mmio_class = NULL;
static struct device* mmio_device = NULL;
static void __iomem *led_reg;       //MMIO 매핑된 주소


// LED ON OFF 제어 함수
static ssize_t mmio_write(struct file* filep, const char *buffer, size_t len, loff_t *offset) {
    unsigned int value;

    if(copy_from_user(&value, buffer, sizeof(value))) {
        return -EFAULT;
    }

    iowrite32(value, led_reg);   // LED 레지스터에 값 쓰기
    printk(KERN_INFO "[MMIO LED] LED 상태 변경 [%d]\n", value);

    return len;
}

//LED 상태 일기 함수
static ssize_t mmio_read(struct file* filep, char *buffer, size_t len, loff_t *offset) {
    unsigned int value = ioread32(led_reg);   // LED 레지스터의 값

    if(copy_to_user(buffer, &value, sizeof(value))) {
        return -EFAULT;
    }

    printk(KERN_INFO "[MMIO LED] LED 상태  [%d]\n", value);
    return sizeof(value);
}


//파일 구조체
static struct file_operations fops = {
    .write = mmio_write,
    .read = mmio_read,
};

//모듈 초기화
static int __init mmio_init(void) {
    printk(KERN_INFO "[MMIO LED] 드라이버 초기화....\n");

    // 커널 가상 주소 매핑
    led_reg = ioremap(LED_REG_BASE, LED_REG_SIZE);
    if(!led_reg) {
        printk(KERN_ALERT "[MMIO LED] 주소 매핑 실패\n");
        return -EIO;
    }
    
    // 캐릭터 디바이스 등록
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    mmio_class = class_create(CLASS_NAME);
    mmio_device = device_create(mmio_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    printk(KERN_INFO "[MMIO LED] 드라이버 초기화 완료\n");

    return 0;
}

//모듈 종료(자원 해제)
static void __exit mmio_exit(void) {
    iounmap(led_reg);   // 매핑된 주소 해제
    device_destroy(mmio_class, MKDEV(majorNumber, 0));
    class_unregister(mmio_class);
    class_destroy(mmio_class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "[MMIO LED] 드라이버 제거\n");
}

module_init(mmio_init);
module_exit(mmio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("soohwan");
MODULE_DESCRIPTION("MMIO LED Driver");
