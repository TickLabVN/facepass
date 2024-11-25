.PHONY: build test clean facepass.so identify.o pam.o

GPP=g++ -fPIC -fno-stack-protector

build: facepass.so
	sudo mkdir -p /lib/security
	sudo ld -x --shared -o /lib/security/facepass.so facepass.so
facepass.so: identify.o pam.o
	$(GPP) -shared -o facepass.so identify.o pam.o
identify.o:
	$(GPP) -c identify.cpp -o identify.o
pam.o:
	$(GPP) pam.cpp -c -o pam.o


test:
	g++ -o pam_test test.cpp -lpam -lpam_misc
clean:
	rm -f *.o
	