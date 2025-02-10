/*
 * 가상 LED 제어 드라이버
 * 이 코드는 IOCTL 을 사용하여 가상의 LED 를 켜고 끄는 기능을 구현.
 */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>  // 사용자 공간과 커널공간간의 데이터 전송 처리
#include <linux/ioctl.h>
#include <linux/device.h>

#define DEVICE_NAME "led_device" 
#define CLASS_NAME "led_class"   

// IOCTL 명령어 정의 (매직넘버 L)
#define LED_MAGIC 'L'
#define LED_ON _IO(LED_MAGIC, 0)   // 켜기 명령
#define LED_OFF _IO(LED_MAGIC, 1)  // 끄기 명령
                                   //


static int majorNumber;    //디바이스 메이저 번호
static struct class* ledClass = NULL;        // 디바이스 클래스
static struct device* ledDevice = NULL;     // 디바이스 구조체
static int led_status = 0;                  // LED의 상태 (ON/OFF)

//IOCTL 함수 정의
static long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch(cmd) {
        case LED_ON:
            led_status = 1;
            printk(KERN_INFO "[LED DRIVER] LED is turned ON\n");
            break;
        case LED_OFF:
            led_status = 0;
            printk(KERN_INFO "[LED DRIVER] LED is turned OFF\n");
            break;
        default:
            printk(KERN_WARNING "[LED DRIVER] Invalid command\n");
            return -EINVAL;// 유효하지 않은 명령어 처리
    }

    return 0;
}


//파일 연산자 구조체 정의
static struct file_operations fops = {
    .unlocked_ioctl = led_ioctl, // IOCTL 호출 시 led_ioctl 함수 호출
};

// 모듈 초기화 함수 
static int __init led_init(void) {
    printk(KERN_INFO "[LED DRIVER] Initializing the LED driver\n");

    // 메이저 번호 동적 할당
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if(majorNumber < 0){
        printk(KERN_ALERT "[LED DRIVER] Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "[LED_DRIVER] Registerd correctly with major number %d\n", majorNumber);

    //클래스 생성
    ledClass = class_create(CLASS_NAME);
    if(IS_ERR(ledClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "[LED DRIVER] Failed to register a class\n");
        return PTR_ERR(ledClass);
    }
    printk(KERN_INFO "[LED DRIVER] Class registered correctly\n");

    //디바이스 생성
    ledDevice = device_create(ledClass, NULL, MKDEV(majorNumber,0), NULL, DEVICE_NAME);
    if(IS_ERR(ledDevice)) {
        class_destroy(ledClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "[LED DRIVER] Failed to create a device\n");
        return PTR_ERR(ledDevice);
    }

    printk(KERN_INFO "[LED DRIVER] Device created correctly\n");

    return 0;
}


//모듈 제거 함수
static void __exit led_exit(void) {
    device_destroy(ledClass, MKDEV(majorNumber,0)); // 디바이스 제거
    class_unregister(ledClass);                     // 클래스 등록 해제
    class_destroy(ledClass);                        // 클래스 제거  
    unregister_chrdev(majorNumber, DEVICE_NAME);    // 메이저 번호 제거
    printk(KERN_INFO "[LED DRIVER] Unregistered correctly\n");
}

module_init(led_init); // 모듈 초기화 함수 원리
module_exit(led_exit); // 모듈 제거 함수 원리

MODULE_LICENSE("GPL");
MODULE_AUTHOR("soohwan");
MODULE_DESCRIPTION("Virtual LED driver");
MODULE_VERSION("1.0");

