.PHONY: build test clean pam.o identify.o

CXX=g++ -std=c++17 -fPIC
CXXFLAGS=-I/usr/include/opencv4

build: pam.o identify.o
	# ld -r pam.o identify.o -o facepass.o
	$(CXX) -shared pam.o identify.o -o pam_facepass.so
	sudo mkdir -p /lib/security
	sudo ld -x --shared -o /lib/security/pam_facepass.so pam_facepass.so

pam.o: 
	$(CXX) $(CXXFLAGS) -c pam.cc -o pam.o
identify.o:
	$(CXX) $(CXXFLAGS) -c identify.cc -o identify.o

test:
	g++ -o pam_test test.cc -lpam -lpam_misc
clean:
	rm -f *.o *.so