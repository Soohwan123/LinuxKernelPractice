#✅ Procfs란?
/proc 디렉터리는 리눅스의 가상 파일 시스템.
커널 정보를 읽거나 값을 설정하는 용도로 사용됨.
cat 명령어로 커널 데이터를 확인할 수 있음.
💻 실습: /proc/myproc 파일을 만들어 커널 데이터를 출력하고, 사용자 입력을 받도록 구현
📌 하는 일

/proc/myproc 파일을 만들고, 파일을 읽으면 "Hello from kernel!"을 반환.
echo 명령어로 데이터를 보내면 커널 로그(dmesg)에 출력.


 컴파일 및 실행
Makefile 생성 (Makefile)

obj-m += myproc.o

all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
컴파일

make
모듈 로드

sudo insmod myproc.ko
테스트

cat /proc/myproc   # 커널 메시지 확인
echo "Hello, kernel!" > /proc/myproc  # 커널에 데이터 보내기
커널 로그 확인

dmesg | tail -10
모듈 제거

sudo rmmod myproc
