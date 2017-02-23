CC=icpc
CFLAG=-g -Wall -std=c++11 -L /usr/local/lib -lboost_thread -lboost_system
BIN=Solution

SRC=main.cpp Para.cpp Para.h Single.cpp Single.h STT.cpp STT.h

all: $(OBJ)
	$(CC) $(CFLAG) $(SRC) -o $(BIN)

clean:
	rm -f *.o
