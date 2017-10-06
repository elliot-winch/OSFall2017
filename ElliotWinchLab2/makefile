CC = gcc

default: main

main: 
	$(RM) main *.o *~
	gcc -c OutputBuilder.c
	gcc -c FCFS.c
	gcc main.c OutputBuilder.o FCFS.o -o main

clean:
	$(RM) main *.o *~