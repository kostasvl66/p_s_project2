#!/bin/bash

#Clearing output file before writing to it
> test_data.txt

#Running sample executions of program
#The number of sample executions is given as input when script is called
for ((i=1;i<=$1;i++))
do
    make run
done

./test $1
