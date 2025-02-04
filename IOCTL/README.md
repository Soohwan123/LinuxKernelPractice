IOCTL (입출력 제어)
✅ IOCTL이란?
ioctl은 사용자 공간 프로그램과 커널 모듈 간의 데이터를 교환하는 강력한 방법.
파일 읽기/쓰기(read/write)보다 더 정교한 커널 제어 명령을 전달할 수 있음.
디바이스 드라이버에서 하드웨어 제어 명령을 전달하는 데 많이 사용됨.
📌 사용자가 ioctl을 사용하여 커널에 특정 명령을 보내도록 구현할 것.

📌 2.1 IOCTL 커널 모듈 구현
📌 목표

/dev/ioctl_device 디바이스 파일을 생성.
사용자 프로그램에서 ioctl을 호출하면, 커널이 해당 명령을 실행.
컴파일 및 실행
make
sudo insmod myioctl.ko
3️⃣ /dev/ioctl_device 디바이스 파일 생성
sudo mknod /dev/ioctl_device c <major_number> 0
🔹 <major_number>는 dmesg | tail -10에서 확인 가능



gcc test_ioctl.c -o test_ioctl
sudo ./test_ioctl
dmesg | tail -10  # 커널 로그 확인.
