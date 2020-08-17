OBJS	= main.o shell.o
SOURCE	= main.c shell.c
HEADER	= shell.h strings.h
OUT	= NSH
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

shell.o: shell.c
	$(CC) $(FLAGS) shell.c 


clean:
	rm -f $(OBJS) $(OUT)