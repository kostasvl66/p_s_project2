CFLAGS = -g -Wall -lpthread 
LIBFLAGS = -lpthread -lm

D = 10  #ARRAY_DIMENSION
Z = 60   #ZERO_PERCENTAGE
R = 5    #REPETITIONS
T = 4    #THREAD_NUM


.SILENT:

main: main.c
	echo "Compiling main"
	gcc $(CFLAGS) main.c -o main $(LIBFLAGS)

run: main
	echo "Running main"
	./main $(D) $(Z) $(R) $ $(T)

clean: main
	echo "Cleaning"
	rm main
