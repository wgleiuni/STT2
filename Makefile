CC=g++
CFLAG=-O2 -Wall -std=c++11
BIN=Solution

SRC=main.cpp

all: $(OBJ)
	$(CC) $(CFLAG) $(SRC) -o $(BIN)

clean:
	rm -f *.o
