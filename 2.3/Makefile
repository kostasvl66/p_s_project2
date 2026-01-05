CC = gcc
CFLAGS = -Wall -O2 -fopenmp
ARGS = 10 serial

PROGRAM = sort

$(PROGRAM): sort.c
	@$(CC) $(CFLAGS) $< -o $@

run: $(PROGRAM)
	./$(PROGRAM) $(ARGS)

clean:
	@rm -f *.o $(PROGRAM)