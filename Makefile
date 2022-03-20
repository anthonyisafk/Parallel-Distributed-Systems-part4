CC = gcc
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang
OUT = -o
BUILD = build/
INCLUDE = util/seq_helpers.c
PTHREAD = -lpthread
CILK = -fopencilk

default: all

.PHONY = clean

axpy_seq:
	$(CC) axpy_seq.c $(OUT) $(BUILD)axpy_seq $(INCLUDE)

sum_seq:
	$(CC) sum_seq.c $(OUT) $(BUILD)sum_seq $(INCLUDE)

sum_pthread:
	$(CC) sum_pthread.c $(OUT) $(BUILD)sum_pthread $(INCLUDE) $(PTHREAD)

all: axpy_seq sum_seq sum_pthread

all_clean: clean axpy_seq sum_seq sum_pthread

clean:
	rm $(BUILD)axpy_seq $(BUILD)sum_seq $(BUILD)sum_pthread