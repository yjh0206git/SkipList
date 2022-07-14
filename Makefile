debug: test.cpp
	g++ -std=c++14 test.cpp src/*.cpp -I./include -pthread
	g++ -std=c++14 stress_test.cpp src/*.cpp -I./include -pthread
