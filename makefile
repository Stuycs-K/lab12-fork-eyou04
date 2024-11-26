default: run

fork.o: fork.c
	@gcc -c fork.c

compile: fork.o
	@gcc -o runme fork.o -Wall

run: compile 
	@./runme

clean:
	rm -f *.o runme