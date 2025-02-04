#Sysfs (시스템 파일 시스템)
✅ Sysfs란?
/sys 디렉터리는 커널 객체 및 속성을 관리하는 가상 파일 시스템.
cat으로 값을 읽거나 echo로 값을 설정할 수 있음.
💻 실습: /sys/kernel/mydevice/myattr 속성을 만들어 사용자 입력을 커널 로그로 출력
📌 하는 일

/sys/kernel/mydevice/myattr을 만들고, 값을 읽거나 쓸 수 있도록 구현.


컴파일 및 실행
make
sudo insmod mysysfs.ko
테스트
# 값 읽기
cat /sys/kernel/mydevice/myattr

# 값 설정
echo 42 > /sys/kernel/mydevice/myattr

# 커널 로그 확인
dmesg | tail -10
모듈 제거
sudo rmmod mysysfs
