
BIN=main
DIFFFILE=ok.txt

CC=$(CXX)

CXXFLAGS=-MMD -MP
CFLAGS=-MMD -MP
#CXXFLAGS=


SRC_FILES=$(wildcard *.cpp)

all: $(BIN)

run: $(BIN)
	./$<

diff: $(BIN)
	./$< > diff.txt
	meld ok.txt diff.txt

install:

-include $(patsubst %.cpp,%.d,$(SRC_FILES))

main: $(patsubst %.cpp,%.o,$(SRC_FILES))

clean:
	rm -f *.o $(BIN) *.d
