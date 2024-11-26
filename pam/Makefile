.PHONY: build test clean pam.o identify.o

CXX=g++ -fPIC -std=c++17
CXXFLAGS=`pkg-config opencv4 --cflags --libs`

build: pam.o identify.o
	$(CXX) -shared pam.o identify.o -o facepass.o $(CXXFLAGS)
	sudo mkdir -p /lib/security
	sudo mv facepass.o /lib/security/pam_facepass.so

pam.o: 
	$(CXX) -c pam.cc -o pam.o $(CXXFLAGS)
identify.o:
	$(CXX) -c identify.cc -o identify.o $(CXXFLAGS) 

test:
	g++ -o pam_test test.cc -lpam -lpam_misc
clean:
	rm -f *.o *.so