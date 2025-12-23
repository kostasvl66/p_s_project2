CFLAGS = -g -Wall -lpthread 

D = 100  #ARRAY_DIMENSION
Z = 60   #ZERO_PERCENTAGE
R = 5    #REPETITIONS
T = 4    #THREAD_NUM


.SILENT:

main:
	echo "Compiling main"
	gcc $(CFLAGS) main.c -o main

run: clean main
	echo "Running main"
	./main $(D) $(Z) $(R) $ $(T)

clean:
	echo "Cleaning"
	rm main
