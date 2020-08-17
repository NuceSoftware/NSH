OBJS	= main.o shell.o
SOURCE	= src/main.c src/shell.c
HEADER	= src/shell.h src/strings.h
OUT	= NSH
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: src/main.c
	$(CC) $(FLAGS) src/main.c

shell.o: src/shell.c
	$(CC) $(FLAGS) src/shell.c


clean:
	rm -f $(OBJS) $(OUT)
