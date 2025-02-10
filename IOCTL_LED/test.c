#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define LED_MAGIC 'L'
#define LED_ON _IO(LED_MAGIC, 0)
#define LED_OFF _IO(LED_MAGIC, 1)

int main() {
    int fd = open("/dev/led_device", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    printf("Turning LED ON...\n");
    ioctl(fd, LED_ON);

    sleep(2);

    printf("Turning LED OFF...\n");
    ioctl(fd, LED_OFF);

    close(fd);
    return 0;
}

