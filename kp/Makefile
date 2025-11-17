CXX = g++
CXXFLAGS = -std=gnu++17 -Wall

main: main.cpp minimp3.h minimp3_ex.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.exe

benchmark: benchmark.cpp minimp3.h minimp3_ex.h
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark.exe

clean:
	rm -f main.exe benchmark.exe output.txt diff.txt

.PHONY: clean

