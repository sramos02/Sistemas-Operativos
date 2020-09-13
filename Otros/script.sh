#!/bin/bash

if [ -f main.c ]; then
	echo "No se ha encontrado el fichero base"
	return 1;
fi

if [ -x ./main ]; then
	rm main
	rm main.o
fi

gcc -c main.c
gcc -o main main.o -pthread
clear
./main
