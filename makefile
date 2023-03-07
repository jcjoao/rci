PROJECT_NAME = cot #target file name

CC = gcc #compiler
#CFLAGS = -Wall -std=c11 -g #-O3

OBJECTS = main.o #objects

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROJECT_NAME)

main.o: main.c

clean:
	rm -f $(PROJECT_NAME) *.o