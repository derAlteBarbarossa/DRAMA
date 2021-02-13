SRC = drama.c
OBJ = $(addsuffix .o, $(basename $(SRC)))
TARGET = $(basename $(OBJ))
CC = gcc
CCFLAGS = 

all: $(TARGET)

drama: drama.o util.o
	${CC} ${CCFLAGS} -o $@ drama.o util.o

util.o: util.c util.h
	${CC} ${CCFLAGS} -c $<

.c.o:
	${CC} ${CCFLAGS} -c $<

clean:
	rm -f *.o $(TARGET) 
