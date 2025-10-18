FLAGS = -Wall
CC = gcc
EXECS = main


main: main.o
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $<

clean:
	rm -f $(EXECS)
	rm -f *.o