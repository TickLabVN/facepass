.PHONY: build test clean pam.o identify.o

CXX=g++ -fPIC -std=c++17
CXXFLAGS=`pkg-config opencv4 --cflags --libs`

build: pam.o identify.o
	$(CXX) -shared pam.o identify.o -o facepass.o
	sudo mkdir -p /lib/security
	sudo mv facepass.o /lib/security/pam_facepass.so

pam.o: 
	$(CXX) $(CXXFLAGS) -c pam.cc -o pam.o
identify.o:
	$(CXX) $(CXXFLAGS) -c identify.cc -o identify.o

test:
	g++ -o pam_test test.cc -lpam -lpam_misc
clean:
	rm -f *.o *.so