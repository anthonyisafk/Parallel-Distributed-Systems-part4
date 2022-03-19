CC = gcc
OUT = -o
BUILD = build/
INCLUDE = util/seq_helpers.c

default: all

.PHONY = clean

axpy_seq:
	$(CC) axpy_seq.c $(OUT) $(BUILD)axpy_seq $(INCLUDE)

sum_seq:
	$(CC) sum_seq.c $(OUT) $(BUILD)sum_seq $(INCLUDE)

all: axpy_seq sum_seq

all_clean: clean axpy_seq sum_seq

clean:
	rm $(BUILD)axpy_seq $(BUILD)sum_seq