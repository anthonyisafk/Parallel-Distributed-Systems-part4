CC = gcc
CILKCC = /usr/local/OpenCilk-9.0.1-Linux/bin/clang -w # suppress C99 warnings
OUT = -o
BUILD = build/
SRC = src/
SEQ_INCLUDE = $(SRC)util/seq_helpers.c
CILK_INCLUDE = $(SRC)util/cilk_helpers.c
PTHREAD = -lpthread
CILK = -fcilkplus -O3
MAKEFLAGS += --silent
NOTIFY = echo " >> Built target " $@

default: all

.PHONY = clean
.PHONY = build
.PHONY = all

build:
	mkdir -p build

axpy_seq:
	make build
	$(CC) $(SRC)axpy_seq.c $(OUT) $(BUILD)axpy_seq $(SEQ_INCLUDE)
	$(NOTIFY)

axpy_pthread:
	make build
	$(CC) $(SRC)axpy_pthread.c $(OUT) $(BUILD)axpy_pthread $(SEQ_INCLUDE) $(PTHREAD)
	$(NOTIFY)

axpy_cilk:
	make build
	$(CILKCC) $(SRC)axpy_cilk.c $(OUT) $(BUILD)axpy_cilk $(SEQ_INCLUDE) $(CILK_INCLUDE) $(CILK)
	$(NOTIFY)

sum_seq:
	make build
	$(CC) $(SRC)sum_seq.c $(OUT) $(BUILD)sum_seq $(SEQ_INCLUDE)
	$(NOTIFY)

sum_pthread:
	make build
	$(CC) $(SRC)sum_pthread.c $(OUT) $(BUILD)sum_pthread $(SEQ_INCLUDE) $(PTHREAD)
	$(NOTIFY)

sum_cilk:
	make build
	$(CILKCC) $(SRC)sum_cilk.c $(OUT) $(BUILD)sum_cilk $(SEQ_INCLUDE) $(CILK_INCLUDE) $(CILK)
	$(NOTIFY)

axpy_cilk2:
	make build
	$(CILKCC) $(SRC)axpy_cilk2.c $(OUT) $(BUILD)axpy_cilk2 $(SEQ_INCLUDE) $(CILK_INCLUDE) $(CILK)
	$(NOTIFY)

axpy_pthread2:
	make build
	$(CC) $(SRC)axpy_pthread2.c $(OUT) $(BUILD)axpy_pthread2 $(SEQ_INCLUDE) $(PTHREAD)
	$(NOTIFY)

multicilk:
	make build
	$(CILKCC) $(SRC)multi-cilk.c $(OUT) $(BUILD)multicilk $(SEQ_INCLUDE) $(CILK_INCLUDE) $(PTHREAD) $(CILK)
	$(NOTIFY)

all: axpy_seq axpy_pthread axpy_cilk sum_seq sum_pthread sum_cilk axpy_cilk2 axpy_pthread2 multicilk

all_clean: clean all

clean:
	rm -f $(BUILD)*