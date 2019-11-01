
CC=$(CXX)

CXXFLAGS=-MMD -MP
CFLAGS=-MMD -MP
#CXXFLAGS=

SRC_FILES=$(wildcard *.cpp)

all: main

run: main
	./$<

install:

-include $(patsubst %.cpp,%.d,$(SRC_FILES))

main: $(patsubst %.cpp,%.o,$(SRC_FILES))

clean:
	rm -f *.o main *.d
