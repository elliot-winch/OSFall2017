CC = gcc

default: main

main: 
	gcc -c ProcessMethods.c
	gcc -c OutputBuilder.c
	gcc -c FCFS.c
	gcc -c RR.c
	gcc -c SJF.c
	gcc -c HPRN.c
	gcc main.c OutputBuilder.o FCFS.o RR.o SJF.c HPRN.c ProcessMethods.o -o main

clean:
	$(RM) main *.o *~