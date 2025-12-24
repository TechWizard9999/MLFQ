# MLFQ Scheduler Makefile
# Run 'make' to compile, './mlfq' to run

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = mlfq
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
