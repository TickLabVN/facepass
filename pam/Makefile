.PHONY: build test clean pam.o identify.o

CXX=g++ -fPIC

# build: pam.o
# 	sudo ld -x --shared pam.o -o /usr/lib/x86_64-linux-gnu/security/pam_facepass.so

build: pam.o identify.o
	ld -r pam.o identify.o -o facepass.o
	sudo ld -x --shared facepass.o -o /usr/lib/x86_64-linux-gnu/security/pam_facepass.so

pam.o: 
	$(CXX) -c pam.cc -o pam.o
identify.o:
	$(CXX) -c identify.cc -o identify.o

test:
	g++ -o pam_test test.cc -lpam -lpam_misc
clean:
	rm -f *.o *.so
	