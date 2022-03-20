CC = gcc
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang
OUT = -o
BUILD = build/
SEQ_INCLUDE = util/seq_helpers.c
CILK_INCLUDE = util/cilk_helpers.c
PTHREAD = -lpthread
CILK = -fcilkplus

default: all

.PHONY = clean

axpy_seq:
	mkdir -p build
	$(CC) axpy_seq.c $(OUT) $(BUILD)axpy_seq $(SEQ_INCLUDE)

axpy_pthread:
	mkdir -p build
	$(CC) axpy_pthread.c $(OUT) $(BUILD)axpy_pthread $(SEQ_INCLUDE) $(PTHREAD)

axpy_cilk:
	mkdir -p build
	$(CILKCC) -w axpy_cilk.c $(OUT) $(BUILD)axpy_cilk $(SEQ_INCLUDE) $(CILK_INCLUDE) $(CILK)

sum_seq:
	mkdir -p build
	$(CC) sum_seq.c $(OUT) $(BUILD)sum_seq $(SEQ_INCLUDE)

sum_pthread:
	mkdir -p build
	$(CC) sum_pthread.c $(OUT) $(BUILD)sum_pthread $(SEQ_INCLUDE) $(PTHREAD)

sum_cilk:
	mkdir -p build
	$(CILKCC) -w sum_cilk.c $(OUT) $(BUILD)sum_cilk $(SEQ_INCLUDE) $(CILK_INCLUDE) $(CILK)

all: axpy_seq axpy_pthread axpy_cilk sum_seq sum_pthread sum_cilk

all_clean: clean axpy_seq axpy_pthread axpy_cilk sum_seq sum_pthread sum_cilk

clean:
	rm -f $(BUILD)axpy_seq $(BUILD)axpy_pthread $(BUILD)axpy_cilk $(BUILD)sum_seq $(BUILD)sum_pthread $(BUILD)sum_cilk