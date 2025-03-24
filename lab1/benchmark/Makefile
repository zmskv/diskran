FLAGS = -std=c++20
TEST_FILE = in.txt
TIME_CMD = grep "time"

.PHONY: all clean test_count test_std test

all: countSort stdSort

countSort: 
	g++ $(FLAGS)  -o countSort countSort.cpp

stdSort: 
	g++ $(FLAGS)  -o stdSort stdSort.cpp

test_count: 
	@echo "Testing count sort..."
	@./countSort < $(TEST_FILE) 

test_std: 
	@echo "Testing std::sort..."
	@./stdSort < $(TEST_FILE) 

test: test_count test_std

clean:
	rm -f countSort stdSort