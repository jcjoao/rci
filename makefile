PROJECT_NAME = cot #target file name

CC = gcc #compiler
CFLAGS = -Wall

OBJECTS = main.o udptcp.o join.o djoin.o names.o menu.o #objects

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROJECT_NAME)

main.o: main.c struct.h
udptcp.o: udptcp.c udptcp.h
join.o: join.c join.h
djoin.o: djoin.c djoin.h
names.o: names.c names.h
menu.o: menu.c menu.h

clean:
	rm -f $(PROJECT_NAME) *.o