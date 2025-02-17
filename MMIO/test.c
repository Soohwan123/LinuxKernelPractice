#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/mmio_led"

int main() {
    int fd = open(DEVICE_PATH, O_RDWR);
    unsigned int val;

    // LED 켜기
    val = 1;
    write(fd, &val, sizeof(val));

    // LED 상태 확인
    read(fd, &val, sizeof(val));
    printf("LED 상태: %d\n", val);

    // LED 끄기
    val = 0;
    write(fd, &val, sizeof(val));

    close(fd);
    return 0;
}

