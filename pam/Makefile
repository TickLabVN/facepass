.PHONY: build test clean pam.o identify.o

GPP=g++ -fPIC -fno-stack-protector

build: pam.o identify.o
	$(GPP) -shared -o facepass.o pam.o identify.o
	# sudo ld -x --shared -o /usr/lib/x86_64-linux-gnu/security/facepass.so facepass.o

pam.o: 
	$(GPP) -c pam.cpp
identify.o:
	$(GPP) -c identify.cpp 


test:
	g++ -o pam_test test.cpp -lpam -lpam_misc
clean:
	rm -f *.o
	