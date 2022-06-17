CC=gcc
FLAGS=-O3 -Wall -Wextra -Wpedantic

all:serial parallel

serial: serial.c
	$(CC) $(FLAGS) serial.c -o serial
#-l OpenCL

parallel: parallel.c
	$(CC) $(FLAGS) parallel.c -o parallel -l OpenCL

