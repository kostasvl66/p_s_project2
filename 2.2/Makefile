CFLAGS = -g -Wall -lpthread 
LIBFLAGS = -lpthread -lm

D = 10  #ARRAY_DIMENSION
Z = 60   #ZERO_PERCENTAGE
R = 5    #REPETITIONS
T = 4    #THREAD_NUM


.SILENT:

main: main.c matrixlib.c
	echo "Compiling main"
	gcc $(CFLAGS) -c main.c -o main.o
	gcc $(CFLAGS) -c matrixlib.c -o matrixlib.o
	gcc $(CFLAGS) -o main main.o matrixlib.o $(LIBFLAGS)

run: main
	echo "Running main"
	./main $(D) $(Z) $(R) $ $(T)

clean: main
	echo "Cleaning"
	rm main
