CC     = gcc
CFLAGS = -g
LIBS   = -lcrypto

PROGS = bn_sample task1 task2 task3

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(PROGS) *.o

.PHONY: all clean
