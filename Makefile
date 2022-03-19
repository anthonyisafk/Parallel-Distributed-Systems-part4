CC = gcc
OUT = -o

default: all

.PHONY = clean

axpy_seq:
	$(CC) axpy_seq.c $(OUT) axpy_seq

sum_seq:
	$(CC) sum_seq.c $(OUT) sum_seq

all: clean axpy_seq sum_seq

clean:
	rm axpy_seq sum_seq