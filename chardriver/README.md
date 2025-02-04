# 문자 디바이스 드라이버 개발
목표:
/dev/mydevice와 같은 디바이스 파일 생성.
open, read, write, close 함수 구현.
사용자 프로그램에서 드라이버 테스트.

Step 1: Major와 Minor 번호
Major 번호: 디바이스의 종류를 식별.
Minor 번호: 동일한 종류의 디바이스 내에서 각각의 장치를 구분.
예제:
/dev/sda1의 Major 번호가 8이라면, 이 번호는 SCSI 디스크 장치를 의미.
Minor 번호는 각 장치(파티션 등)를 식별.


컴파일 및 실행
컴파일:

make
모듈 로드:

sudo insmod mydevice.ko
모듈 확인:

lsmod | grep mydevice
디바이스 파일 생성:

sudo mknod /dev/mydevice c <major_number> 0
<major_number>는 dmesg 출력에서 확인한 Major 번호로 대체.


Step 5: 테스트
읽기 테스트:

cat /dev/mydevice
쓰기 테스트:

echo "Test from user space" > /dev/mydevice
