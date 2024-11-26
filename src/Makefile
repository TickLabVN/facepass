.PHONY: build

CXX=g++ -fPIC -std=c++17
CXXFLAGS=`pkg-config opencv4 --cflags --libs`

build: 
	$(CXX) $(CXXFLAGS) main.cc -o main
