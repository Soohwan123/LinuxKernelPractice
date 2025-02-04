#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MY_IOCTL_CMD _IOW('a', 1, int)

int main() {
    int fd = open("/dev/ioctl_device", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    int value = 123;
    if (ioctl(fd, MY_IOCTL_CMD, &value) < 0) {
        perror("Failed to send IOCTL");
        return -1;
    }

    printf("IOCTL sent successfully\n");
    close(fd);
    return 0;
}

