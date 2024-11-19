build:
	gcc -fPIC -fno-stack-protector -o mypam.o -c main.c
	sudo mkdir -p /lib/security
	sudo ld -x --shared -o /lib/security/mypam.so mypam.o
	rm mypam.o
test:
	gcc -o pam_test test.c -lpam -lpam_misc